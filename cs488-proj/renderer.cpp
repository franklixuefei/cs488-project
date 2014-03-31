#include "renderer.hpp"

#include <cmath>
#include <cstdlib>
#include "formulas.hpp"
void* thread_start(void* data)
{
  Args *args = (Args*) data;
  args->renderer->threaded_render(args->start_row, args->render_size);
  return NULL;
}

Renderer::Renderer(
                   SceneNode* root,
                   int width, int height,
                   const Point3D& eye, const Vector3D& view,
                   const Vector3D& up, double fov,
                   const Colour& ambient,
                   const std::list<Light*>& lights,
                   size_t num_photon_recursion,
             double photon_search_radius,
                   size_t num_search_photons,
                   const std::string& mode,
                   size_t num_threads,
                   const std::string& type,
                   double max_psi,
                   size_t pixelSize
) :
	m_img(Image(width, height, 3)),
    m_bg_img(Image(width, height, 3)),
	m_root(root),
	m_width(width),
	m_height(height),
	m_eye(eye),
	m_view(view),
	m_up(up),
	m_left(up.cross(view)),
	m_fov(fov),
	m_screen_distance(((double)std::max(width, height) / 2.0) / tan(M_PI * fov / 360.0)),
	m_ambient(ambient),
	m_lights(lights),
    m_photon_recursion(num_photon_recursion),
    m_photon_search_radius(photon_search_radius),
    m_num_search_photons(num_search_photons),
    m_render_mode(mode),
    m_num_threads(num_threads),
    m_render_type(type),
    m_max_psi(max_psi),
    m_pixel_size(pixelSize),
    m_pinhole_camera(NULL),
    m_fisheye_camera(NULL)
{
    size_t total_num_init_photons = 0;
    for (std::list<Light*>::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) {
        total_num_init_photons += (*it)->getNumPhotons();
    }
    std::cerr << "total initial number of photons: " << total_num_init_photons << endl;
    m_photon_map = new PhotonMap((int)(total_num_init_photons*m_photon_recursion));
	m_view.normalize();
	m_up.normalize();
	m_left.normalize();
    if (m_render_type == "pinhole") {
        m_pinhole_camera = new PinholeCamera(
            new CameraProperties(m_width, m_height, m_screen_distance, m_view, m_left, m_up, m_pixel_size, m_pixel_size*m_pixel_size)
        );
    } else if (m_render_type == "fisheye") {
        m_fisheye_camera = new FishEyeCamera(
            new CameraProperties(m_width, m_height, m_screen_distance, m_view, m_left, m_up, m_pixel_size, m_pixel_size*m_pixel_size),
            m_max_psi
        );
    } else {
        cerr << "wrong render type: " + m_render_type << endl;
        exit(1);
    }
    
}

Renderer::~Renderer()
{
	delete m_photon_map;
    if (m_pinhole_camera) delete m_pinhole_camera;
    if (m_fisheye_camera) delete m_fisheye_camera;
}

void Renderer::build_photon_map()
{
    Point3D orig;
    Vector3D ray_dir;
    for (std::list<Light*>::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) { // for each light source
        Light *light = *it;
        double x, y, z;
        for (int i = 0; i < light->getNumPhotons(); ++i) {
            SquareLight *squareLight = dynamic_cast<SquareLight*>(light);
            PointLight *pointLight = dynamic_cast<PointLight*>(light);
            if (squareLight) {
                double lightSize = squareLight->getSize();
#warning NOTE: for now only downward lights considered
                x = lightSize * rand() / RAND_MAX - lightSize/2.0;
                y = 0;
                z = lightSize * rand() / RAND_MAX - lightSize/2.0;
                Vector3D offset(x, y, z);
                orig = squareLight->getPosition() + offset;
            } else if (pointLight) {
                orig = pointLight->getPosition();
            }
#warning TODO: more light types here...
            do {
                x = (1.0 * rand() / RAND_MAX - 0.5) * 2.0;
                y = -(1.0 * rand() / RAND_MAX);
                z = (1.0 * rand() / RAND_MAX - 0.5) * 2.0;
            } while (x*x + y*y + z*z > 1.0 || y == 0);
            ray_dir = Vector3D(x, y, z);
            ray_dir.normalize();
            Colour power = light->getPower() * light->getColour();
            m_photon_map->store(power.getRaw(), orig.getRaw(), (-1.0 * ray_dir).getRaw());
            trace_photons(orig, ray_dir, power, 1.0, 0);
        }
        m_photon_map->scale_photon_power(1.0 / (double)light->getNumPhotons());
    }
    m_photon_map->balance();
}

Image Renderer::render()
{
    if (m_render_mode == "all" || m_render_mode == "photon map") {
        build_photon_map();
        cerr << "num stored photons: " << m_photon_map->get_stored_photons() << endl;
    }
    
	std::vector<pthread_t*> threads;
	struct Args* args = new Args[m_num_threads];
	for (unsigned int i = 0 ;i < m_num_threads; ++i) {
		pthread_t* thread = new pthread_t;
		threads.push_back(thread);
		args[i].renderer = this;
		args[i].start_row = i;
		args[i].render_size = m_num_threads;
		pthread_create(thread, NULL, &thread_start, (void *)&args[i]);
	}
	for (unsigned int i = 0; i < threads.size(); ++i) {
		pthread_join(*(threads[i]), NULL);
		delete threads[i];
	}
	delete [] args;
	return m_img;
}

void Renderer::threaded_render(unsigned int start_row, size_t render_size)
{

	for (unsigned int y = start_row; y < m_height; y+=render_size) {
		for (int x = 0; x < m_width; x++) {
		  Colour colour = background(x, y);
            getColourForPixel(x, y, colour);
            m_img(x, y, 0) = colour.R();
            m_img(x, y, 1) = colour.G();
            m_img(x, y, 2) = colour.B();
		}
	}
}

Colour Renderer::background(int x, int y)
{
	return Colour(0.0, 0.0, y * (double)(1.0/(double)m_height));
}

BasicRenderer::BasicRenderer(
                             SceneNode* root,
                             int width, int height,
                             const Point3D& eye, const Vector3D& view,
                             const Vector3D& up, double fov,
                             const Colour& ambient,
                             const std::list<Light*>& lights,
                             size_t num_photon_recursion,
             double photon_search_radius,
                             size_t num_search_photons,
                             const std::string& mode,
                             size_t num_threads,
                             const std::string& type,
                             double max_psi
                             ) :
    Renderer(root,width,height,eye,view,up,fov,ambient,lights,num_photon_recursion,photon_search_radius,num_search_photons,mode,num_threads,type,max_psi,1)

{}

BasicRenderer::~BasicRenderer()
{}

bool BasicRenderer::getColourForPixel(int x, int y, Colour & c)
{
    Vector3D ray_dir;
    Camera *camera = m_pinhole_camera ? m_pinhole_camera : m_fisheye_camera;
    double dx = (double)m_width / 2.0 - (double)x;
	double dy = (double)m_height / 2.0 - (double)y;
    if (camera->gen_ray(dx, dy, ray_dir)) {
        IntersectionPoint ip; // not used
        if (m_root->intersect(m_eye, ray_dir, c, m_lights, m_ambient, this, 1.0, ip)) {
            return true;
        }
    }
    return false;
}

// for photon visualization
bool BasicRenderer::photon_intersect(const Vector3D &ray_dir, Colour &c)
{
    for (int i = 0; i < m_photon_map->get_stored_photons(); ++i) {
        Point3D p = Point3D(
            m_photon_map->getPhotons()[i].pos[0],
            m_photon_map->getPhotons()[i].pos[1],
            m_photon_map->getPhotons()[i].pos[2]
        );
        double t = DBL_MAX;
        if (solve3x1linearSystems(ray_dir, m_eye-Point3D(), p-Point3D(), t)) {
            cerr << t << endl;
            c = Colour(1,0,0);
            return true;
        }
    }
    return false;
}

StochasticRenderer::StochasticRenderer(
                                       SceneNode* root,
                                       int width, int height,
                                       const Point3D& eye, const Vector3D& view,
                                       const Vector3D& up, double fov,
                                       const Colour& ambient,
                                       const std::list<Light*>& lights,
                                       size_t num_photon_recursion,
             double photon_search_radius,
                                       size_t num_search_photons,
                                       const std::string& mode,
                                       size_t num_threads,
                                       const std::string& type,
                                       double max_psi,
                                       size_t pixelSize
                                       ):
    Renderer(root,width,height,eye,view,up,fov,ambient,lights,num_photon_recursion,photon_search_radius,num_search_photons,mode,num_threads,type,max_psi,pixelSize),
    m_pixel_size(pixelSize)
{}

StochasticRenderer::~StochasticRenderer()
{
    
}

bool StochasticRenderer::getColourForPixel(int x, int y, Colour &c)
{
    c = Colour(0,0,0);
    Vector3D ray_dir;
    Camera *camera = m_pinhole_camera ? m_pinhole_camera : m_fisheye_camera;
    double orig_dx = (double)m_width / 2.0 - (double)x;
	double orig_dy = (double)m_height / 2.0 - (double)y;
    
    double stepSize = 1.0 / (double)m_pixel_size;
    
    double totalWeight = 0;
    for (int sub_y = 0; sub_y < m_pixel_size; ++sub_y) {
        for (int sub_x = 0; sub_x < m_pixel_size; ++sub_x) {
            double x_offset = (((double)rand() / (double)RAND_MAX - 0.5) / m_pixel_size) +
            stepSize * (double)sub_x - 0.5 + stepSize / 2.0;
            double y_offset = (((double)rand() / (double)RAND_MAX - 0.5) / m_pixel_size) +
            stepSize * (double)sub_x - 0.5 + stepSize / 2.0;
            
            double dx = orig_dx + x_offset;
            double dy = orig_dy + y_offset;
            
            Colour sample_colour = background(x, y);
            if (camera->gen_ray(dx, dy, ray_dir)) {
                IntersectionPoint ip; // not used
                if  (!m_root->intersect(m_eye, ray_dir, sample_colour, m_lights, m_ambient, this, 1.0, ip))
                {
                    continue;
                }
                double distSquared = x_offset * x_offset + y_offset * y_offset;
                double weight = (1.0 / sqrt(2.0 * M_PI)) * exp(-1.0/2.0 * distSquared);
                c = c + weight * sample_colour;
                totalWeight += weight;
            } else {
                continue;
            }
        }
    }
    if (totalWeight) {
        c = (1.0 / totalWeight) * c;
    } else {
        c = background(x, y);
    }
    return true;
}


/*===================================Photon Mapping ==============================*/

PhotonMap* Renderer::getPhotonMap() const
{
    return m_photon_map;
}

double Renderer::getPhotonSearchRadius() const
{
    return m_photon_search_radius;
}

size_t Renderer::getNumSearchPhotons() const
{
    return m_num_search_photons;
}

const string & Renderer::getRenderMode() const
{
    return m_render_mode;
}

size_t Renderer::getNumPhotonRecursion() const
{
    return m_photon_recursion;
}

void Renderer::trace_photons(Point3D orig, Vector3D dir, Colour power, double refl_id, size_t recur_depth)
{
    if (recur_depth > m_photon_recursion) {
        return;
    }
    IntersectionPoint ip;
    if (!m_root->intersect(orig, dir, ip, refl_id)) {
        return;
    }

    Vector3D nl = ip.m_orig_normal.dot(dir) < 0 ? ip.m_orig_normal : -1.0 * ip.m_orig_normal;
    
    double hitMatReflID = ip.m_owner->get_material()->getRefractiveIndex();
    double reflectance = 1.0; // default the reflectance to 1.0 (full reflection)
    double transmission = 1.0 - reflectance;
    Vector3D srefl_dir = Formulas::perfectReflection(nl, dir); // specular reflection
    srefl_dir.normalize();
    Colour kd = ip.m_owner->get_material()->getDiffuse(ip.m_owner->get_primitive(), ip.m_orig_point);
    if (hitMatReflID != DBL_MAX) { // translucent object
#warning ray is hitting out of the object (to the air), assuming no two intact objects.
        Vector3D t_dir;
        double na, nb;
        bool into = true;
        if ((into = ip.m_normal.dot(dir) < 0)) { // ray going in
            na = refl_id;
            nb = hitMatReflID;
        } else {
            na = hitMatReflID;
            nb = 1.0;
        }
        double nnt, ddn;
        
        bool canRefract = Formulas::SnellRefraction(dir, ip.m_normal, na, nb, t_dir, nnt, ddn);

        if (!canRefract) { // total internal reflection);
            trace_photons(ip.m_point, srefl_dir, power, refl_id, recur_depth + 1);
        } else { // can refract
            t_dir.normalize();
            // calculate reflectance using Fresnel's Formula
            reflectance = Formulas::schlickApproxFresnelReflectance(1.0, hitMatReflID, (into ? -1.0 * ddn : t_dir.dot(ip.m_normal)));
            transmission = 1.0 - reflectance;
            // Russion Roulette
            double p = 0.25 + 0.5 * reflectance;
            double reflP = reflectance / p;
            double transP = transmission / (1.0 - p);
            Vector3D refl_dir = Formulas::perfectReflection(ip.m_normal, dir);
            if (recur_depth > 2) {
                double rand1 = 1.0 * rand() / RAND_MAX;
                if (rand1 < p) {
                    trace_photons(ip.m_point, refl_dir, reflP * power, refl_id, recur_depth + 1);
                } else {
                    trace_photons(ip.m_point, t_dir, transP * power * (into?kd:Colour(1,1,1)), into?hitMatReflID:1.0, recur_depth+1);
                }
            } else {
                trace_photons(ip.m_point, refl_dir, reflP * power, refl_id, recur_depth + 1);
                trace_photons(ip.m_point, t_dir, transP * power, into?hitMatReflID:1.0, recur_depth + 1);
            }
        }
    } else { // non-translucent, specular or diffuse depending on kd_avg
//        trace_photons(ip.m_point, refl_dir, reflP * power, refl_id, recur_depth + 1);
        
        double kd_avg = (kd.R() + kd.G() + kd.B()) / 3.0;
        double refl_rand = 1.0 * rand() / RAND_MAX;
        if (refl_rand < kd_avg) { // diffuse reflection
            /*if (recur_depth > 0)*/ m_photon_map->store((kd*(1.0 / kd_avg)*power).getRaw(), ip.m_point.getRaw(), dir.getRaw());
#warning can also use cosine weight reflection.
            Vector3D drefl_dir = Formulas::pureRandomDiffuseReflection(nl);
            drefl_dir.normalize();
            // power of each photon remains unchanged, but the amount of photons decreases each time.
            trace_photons(ip.m_point, drefl_dir, power, refl_id, recur_depth+1);
        } else { // specular reflection
            trace_photons(ip.m_point, srefl_dir, power, refl_id, recur_depth+1);
        }
    }
    
    
}



SceneNode *Renderer::get_root() const
{
	return m_root;
}