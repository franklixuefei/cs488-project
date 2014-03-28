#include "renderer.hpp"

#include <cmath>
#include <cstdlib>

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
    string type, // pinhole or fisheye
    size_t pixelSize
) :
	m_img(Image(width, height, 3)),
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
    m_photon_map(new PhotonMap(MAX_NUM_PHOTONS)),
    m_pinhole_camera(NULL),
    m_fisheye_camera(NULL)
{
	m_view.normalize();
	m_up.normalize();
	m_left.normalize();
    if (type == "pinhole") {
        m_pinhole_camera = new PinholeCamera(
            new CameraProperties(m_width, m_height, m_screen_distance, m_view, m_left, m_up, pixelSize, pixelSize*pixelSize)
        );
    } else {
        m_fisheye_camera = new FishEyeCamera(
            new CameraProperties(m_width, m_height, m_screen_distance, m_view, m_left, m_up, pixelSize, pixelSize*pixelSize),
            180.0 // max_psi
        );
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
    Light hard_coded_light;
    Point3D orig;
    Vector3D ray_dir;
#warning need to be plugged in lua parser
    hard_coded_light.position = Point3D(0, 2.45, 4.167);
    double x, y, z;
    for (int i = 0; i < INIT_NUM_PHOTONS; ++i) {
        x = 1.5 * rand() / RAND_MAX - 0.75; // -0.75 - 0.75
        y = 0; // -0.75 - 0.75
        z = 1.5 * rand() / RAND_MAX - 0.75; // -0.75 - 0.75
        Vector3D offset(x, y, z);
        orig = hard_coded_light.position + offset;
        do {
            x = (1.0 * rand() / RAND_MAX - 0.5) * 2.0;
            y = -(1.0 * rand() / RAND_MAX);
            z = (1.0 * rand() / RAND_MAX - 0.5) * 2.0;
        } while (x*x + y*y + z*z > 1.0 || y == 0);
        
        ray_dir = Vector3D(x, y, z);
        ray_dir.normalize();
        Colour power = Colour(100.0, 100.0, 100.0);
//        m_photon_map->store(power.getRaw(), orig.getRaw(), Vector3D().getRaw());
        trace_photons(orig, ray_dir, power, 1.0, 0);
    }
    m_photon_map->scale_photon_power(1.0 / (double)INIT_NUM_PHOTONS);
    m_photon_map->balance();
}


Image Renderer::render()
{
//    build_photon_map();
    cerr << "photons: " << m_photon_map->get_stored_photons() << endl;
//    Photon* photons = m_photon_map->getPhotons();
//    for (int i = 0; i < m_photon_map->get_stored_photons(); ++i) {
//        cerr << photons[i].pos[0] << ", " << photons[i].pos[1] <<  ", " << photons[i].pos[2] << endl;
//    }
    
	size_t numThreads = 32;
	std::vector<pthread_t*> threads;
	struct Args* args = new Args[numThreads];
	unsigned int i = 0;
	for (;i < numThreads; ++i) {
		pthread_t* thread = new pthread_t;
		threads.push_back(thread);
		args[i].renderer = this;
		args[i].start_row = i;
		args[i].render_size = numThreads;
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
              // Image size
              int width, int height,
              // Viewing parameters
              const Point3D& eye, const Vector3D& view,
              const Vector3D& up, double fov,
              // Lighting parameters
              const Colour& ambient,
              const std::list<Light*>& lights,
                const string& type
              ) :
    Renderer(root,width,height,eye,view,up,fov,ambient,lights, type, 1)
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
        if (m_root->intersect(m_eye, ray_dir, c, m_lights, m_ambient, this, 1.0)) {
//        return photon_intersect(ray_dir, c);
            IntersectionPoint ip;
            double irrad[3];
            if (m_root->intersect(m_eye, ray_dir, ip, 1.0)) {
//                m_photon_map->irradiance_estimate(irrad, ip.m_point.getRaw(), ip.m_normal.getRaw(), 0.1, 200);
#warning need to scale up the colour a bit...
//                c = c + Colour(irrad[0], irrad[1], irrad[2]);
                //            cerr << c << endl;
                return true;
            }
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
                   // Image size
                   int width, int height,
                   // Viewing parameters
                   const Point3D& eye, const Vector3D& view,
                   const Vector3D& up, double fov,
                   // Lighting parameters
                   const Colour& ambient,
                   const std::list<Light*>& lights,
                    const string& type,
                    size_t pixelSize
                ):
    Renderer(root,width,height,eye,view,up,fov,ambient,lights, type, pixelSize),
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
                if  (!m_root->intersect(m_eye, ray_dir, sample_colour, m_lights, m_ambient, this, 1.0))
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

void Renderer::trace_photons(Point3D orig, Vector3D dir, Colour power, double refl_id, size_t recur_depth)
{
    if (recur_depth > MAX_PHOTON_RECURSION) {
        return;
    }
    IntersectionPoint ip;
    if (!m_root->intersect(orig, dir, ip, refl_id)) {
        return;
    }
//    cerr << ip.m_owner->getName() << endl;
    Colour kd = ip.m_owner->get_material()->getDiffuse(ip.m_owner->get_primitive(), ip.m_orig_point);
    double kd_avg = (kd.R() + kd.G() + kd.B()) / 3.0;
    double refl_rand = 1.0 * rand() / RAND_MAX;
    if (refl_rand < kd_avg) { // reflect
        if (recur_depth > 0) m_photon_map->store((kd*power).getRaw(), ip.m_point.getRaw(), dir.getRaw());
        Colour refl_power = power * (kd * (1.0 / kd_avg));
//        Vector3D nl = ip.m_orig_normal.dot(dir) < 0 ? ip.m_orig_normal : -1.0 * ip.m_orig_normal;
        Vector3D drefl_dir = calc_diffuse_reflection(ip.m_orig_normal);
        drefl_dir.normalize();
        trace_photons(ip.m_point, drefl_dir, power, refl_id, recur_depth+1);
    }
#warning TODO: refraction(need to set refl_id) and specular reflection using Russian Roulette
}

Vector3D Renderer::calc_diffuse_reflection(const Vector3D &n)
{
/* perfect diffuse version -- not suitable here - should be used while rendering*/
//    Vector3D dir;
//    double rand1 = 1.0 * rand() / RAND_MAX; // [0,1]
//    double rand2 = 1.0 * rand() / RAND_MAX; // [0,1]
//    double theta = 2.0 * M_PI * rand1;
//    double cosPhi = sqrt(rand2);
//    Vector3D w = n, u = (fabs(w[0]) > 0.1 ? Vector3D(0,1,0) : Vector3D(1,0,0)).cross(w);
//    u.normalize();
//    Vector3D v = w.cross(u);
//    dir = cos(theta)*cosPhi*u + sin(theta)*cosPhi*v + sqrt(1-rand2)*w;
//    dir.normalize();
//    return dir;
    
/* pure random version */
    double x, y, z;
    Vector3D dir;
    
    /* reject any direction that is opposite to the surface normal */
    do {
        /* determine new direction by rejection sampling */
        x = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        y = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        z = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        dir = Vector3D(x, y, z);
    } while(x*x + y*y + z*z > 1.0 && dir.dot(n) > SMALL_EPSILON);

    return dir;
}




SceneNode *Renderer::get_root() const
{
	return m_root;
}