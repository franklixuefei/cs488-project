#include "primitive.hpp"
#include "polyroots.hpp"
#include <float.h>
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;
Primitive::~Primitive()
{
}

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces,
           bool isBoundingBox)
: m_verts(verts),
m_faces(faces),
m_bounding_box(NULL)
{
    for (std::vector<Face>::const_iterator it = faces.begin(); it != faces.end(); it++) {
        Vector3D v1 = verts[(*it)[0]] - verts[(*it)[1]];
        Vector3D v2 = verts[(*it)[2]] - verts[(*it)[1]];
        Vector3D normal = v2.cross(v1);
        
        std::vector<Point3D> vertices;
        for (Face::const_iterator it2 = it->begin(); it2 != it->end(); it2++) {
            vertices.push_back(verts[*it2]);
        }
        
        m_polygons.push_back(new Polygon(vertices, normal));
    }
    if (!isBoundingBox) {
        // create bounding box
        std::vector<Point3D> vertices;
        Point3D min, max;
        findExtremePoints(min, max);
        double box_size_x = (max[0] - min[0]);
        double box_size_y = (max[1] - min[1]);
        double box_size_z = (max[2] - min[2]);
        vertices.push_back(Point3D(min[0], min[1], min[2]));
        vertices.push_back(Point3D(min[0] + box_size_x, min[1], min[2]));
        vertices.push_back(Point3D(min[0] + box_size_x, min[1] + box_size_y, min[2]));
        vertices.push_back(Point3D(min[0], min[1] + box_size_y, min[2]));
        vertices.push_back(Point3D(min[0], min[1], min[2] + box_size_z));
        vertices.push_back(Point3D(min[0] + box_size_x, min[1], min[2] + box_size_z));
        vertices.push_back(Point3D(min[0] + box_size_x, min[1] + box_size_y, min[2] + box_size_z));
        vertices.push_back(Point3D(min[0], min[1] + box_size_y, min[2] + box_size_z));
        
        std::vector<std::vector<int> > bounding_faces;
        int f[6][4] = {
            {3, 2, 1, 0},   // front
            {0, 1, 5, 4},   // bottom
            {2, 6, 5, 1},   // right
            {7, 3, 0, 4},   // left
            {7, 6, 2, 3},   // top
            {4, 5, 6, 7}  // rear
        };
        for (int i = 0; i < 6; ++i) {
            std::vector<int> face;
            for (int j = 0; j < 4; ++j) {
                face.push_back(f[i][j]);
            }
            bounding_faces.push_back(face);
        }
        m_bounding_box_min = min;
        m_bounding_box_max = max;
        m_bounding_box = new Mesh(vertices, bounding_faces, true);
    }
}

Mesh::~Mesh()
{
    for (std::vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it) {
        delete *it;
    }
    delete m_bounding_box;
}

Vector3D Mesh::getNormal(const Point3D& p) const
{
    return getPolygonForPoint(p)->getNormal();
}

bool Mesh::within_bounding_box(const Point3D &p)
{
    bool x_within = p[0] - m_bounding_box_min[0] >= -SMALL_EPSILON && p[0] - m_bounding_box_max[0] <= SMALL_EPSILON;
    bool y_within = p[1] - m_bounding_box_min[1] >= -SMALL_EPSILON && p[1] - m_bounding_box_max[1] <= SMALL_EPSILON;
    bool z_within = p[2] - m_bounding_box_min[2] >= -SMALL_EPSILON && p[2] - m_bounding_box_max[2] <= SMALL_EPSILON;
    return x_within && y_within && z_within;
}

Polygon* Mesh::getPolygonForPoint(const Point3D& p) const
{
    for (std::vector<Polygon*>::const_iterator it = m_polygons.begin(); it != m_polygons.end(); ++it) {
        if ((*it)->isWithinPolygon(p)) {
            return *it;
        }
    }
#ifdef DEBUG
    std::cerr << "No Polygon found for point: " << p << std::endl;
#endif
    return NULL;
}

void Mesh::findExtremePoints( Point3D &min, Point3D &max)
{
    double cur_min_x, cur_min_y, cur_min_z, cur_max_x, cur_max_y, cur_max_z;
    cur_min_x = cur_min_y = cur_min_z = DBL_MAX;
    cur_max_x = cur_max_y = cur_max_z = -DBL_MAX;
    
    for (int i = 0; i < m_verts.size(); ++i) {
        Point3D p = m_verts[i];
        if (p[0] < cur_min_x) cur_min_x = p[0];
        if (p[0] > cur_max_x) cur_max_x = p[0];
        if (p[1] < cur_min_y) cur_min_y = p[1];
        if (p[1] > cur_max_y) cur_max_y = p[1];
        if (p[2] < cur_min_z) cur_min_z = p[2];
        if (p[2] > cur_max_z) cur_max_z = p[2];
    }
    min = Point3D(cur_min_x, cur_min_y, cur_min_z);
    max = Point3D(cur_max_x, cur_max_y, cur_max_z);
    
}


bool Mesh::intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id)
{
    if (m_bounding_box) {
        IntersectionPoint tempIP = ip;
        if (!within_bounding_box(eye) && !m_bounding_box->intersect(eye, ray, tempIP, DBL_MAX)) {
            return false;
        } else { // for showing bounding boxes
            // ip = temp;
            // ip.m_t -= SMALL_EPSILON;
            // return ip.m_t != DBL_MAX;
        }
    }
    
    
    
    IntersectionPoint minIP;
    minIP.m_t = DBL_MAX;
    
    for (std::vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it) {
        if ((*it)->intersect(eye, ray, ip, refl_id)) {
            if (ip < minIP) {
                minIP = ip;
            }
        }
    }
    ip = minIP;
    if (ip.m_t < SMALL_EPSILON) return false;
    return ip.m_t != DBL_MAX;
}

Point2D Mesh::get2DTextureMapCoordinates(const Point3D &p) const
{
    return getPolygonForPoint(p)->get2DTextureMapCoordinates(p);
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
    std::cerr << "mesh({";
    for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
        if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
        std::cerr << *I;
    }
    std::cerr << "},\n\n     {";
    
    for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
        if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
        std::cerr << "[";
        for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
            if (J != I->begin()) std::cerr << ", ";
            std::cerr << *J;
        }
        std::cerr << "]";
    }
    std::cerr << "});" << std::endl;
    return out;
}

Sphere::Sphere()
{
    m_basic_sphere = new NonhierSphere(Point3D(0,0,0), 1.0);
}

Sphere::~Sphere()
{
    delete m_basic_sphere;
}
Vector3D Sphere::getNormal(const Point3D& p) const
{
    return m_basic_sphere->getNormal(p);
}

bool Sphere::intersect(const Point3D& eye, const Vector3D& ray,  IntersectionPoint &ip, double refl_id)
{
    return m_basic_sphere->intersect(eye, ray, ip, refl_id);
}

Point2D Sphere::get2DTextureMapCoordinates(const Point3D &p) const
{
    return m_basic_sphere->get2DTextureMapCoordinates(p);
}

Cube::Cube()
{
    m_basic_cube = new NonhierBox(Point3D(0,0,0), 1.0);
}

Cube::~Cube()
{
    delete m_basic_cube;
}
Vector3D Cube::getNormal(const Point3D& p) const
{
    return m_basic_cube->getNormal(p);
}

bool Cube::intersect(const Point3D& eye, const Vector3D& ray,  IntersectionPoint &ip, double refl_id)
{
    return m_basic_cube->intersect(eye, ray, ip, refl_id);
}

Point2D Cube::get2DTextureMapCoordinates(const Point3D &p) const
{
    return m_basic_cube->get2DTextureMapCoordinates(p);
}

NonhierSphere::~NonhierSphere()
{
    
}

Vector3D NonhierSphere::getNormal(const Point3D& p) const
{
    Vector3D normal = p - m_pos;
    return normal;
}

bool NonhierSphere::isWithinBound(const Point3D &eye, const Vector3D &ray)
{
    Vector3D eye_center = m_pos - eye;
    double eye_center_dist = eye_center.length();
    double sin_theta = m_radius / eye_center_dist;
    double cos_theta = sqrt(1-pow(sin_theta, 2.0));;
    double actual_cos_theta = (ray.dot(eye_center)) / (ray.length() * eye_center.length());
    return actual_cos_theta - cos_theta > -EPSILON;
}


bool NonhierSphere::intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id)
{
	double A = 0;
	double B = 0;
	double C = 0;
    
	for (int i = 0; i < 3; i++) {
		A += ray[i] * ray[i];
		B += 2.0 * (eye[i] - m_pos[i]) * ray[i];
		C += (eye[i] - m_pos[i]) * (eye[i] - m_pos[i]);
	}
	C -= m_radius * m_radius;
    
    // if (!isWithinBound(eye, ray)) {
    //   return false;
    // }
    
	double roots[2];
	double t = DBL_MAX;
    size_t num = quadraticRoots(A, B, C, roots);
    if (roots[0] < SMALL_EPSILON) roots[0] = DBL_MAX;
    if (roots[1] < SMALL_EPSILON) roots[1] = DBL_MAX;
    t = roots[0] < roots[1] ? roots[0] : roots[1];
    bool hit =  num != 0 && t >= SMALL_EPSILON && t != DBL_MAX;
	if (hit) {
		Point3D p = eye + t * ray;
        // ip.m_point = p;
        ip.m_t = t;
        ip.m_normal = ip.m_orig_normal = getNormal(p);
        ip.m_normal.normalize();
	}
    
	return hit;
}

Point2D NonhierSphere::get2DTextureMapCoordinates(const Point3D &p) const
{
    double phi = atan2(-(double)p[1],-(double)p[0]);
    double u = (phi + M_PI) / (M_PI);
    if (u > 1.0) u = u - 1.0;
    if (u < 0.0) u = u + 1.0;
    double theta = acos(p[2]);
    double v = 1.0 - theta / M_PI;
    if (v > 1.0) v = v - 1.0;
    if (v < 0.0) v = v + 1.0;
    return Point2D(u,v);
}

NonhierBox::NonhierBox(const Point3D& pos, double size):
m_pos(pos), m_size(size)
{
	std::vector<Point3D> vertices;
	vertices.push_back(Point3D(pos[0], pos[1], pos[2]));
	vertices.push_back(Point3D(pos[0] + size, pos[1], pos[2]));
	vertices.push_back(Point3D(pos[0] + size, pos[1] + size, pos[2]));
	vertices.push_back(Point3D(pos[0], pos[1] + size, pos[2]));
	vertices.push_back(Point3D(pos[0], pos[1], pos[2] + size));
	vertices.push_back(Point3D(pos[0] + size, pos[1], pos[2] + size));
	vertices.push_back(Point3D(pos[0] + size, pos[1] + size, pos[2] + size));
	vertices.push_back(Point3D(pos[0], pos[1] + size, pos[2] + size));
    
	std::vector<std::vector<int> > faces;
	int f[6][4] = {
		{3, 2, 1, 0}, 	 // rear
		{0, 1, 5, 4}, 	 // bottom
		{2, 6, 5, 1}, 	 // right
		{7, 3, 0, 4}, 	 // left
		{7, 6, 2, 3}, 	 // top
		{4, 5, 6, 7}	   // front
	};
	for (int i = 0; i < 6; ++i) {
		std::vector<int> face;
		for (int j = 0; j < 4; ++j) {
			face.push_back(f[i][j]);
		}
		faces.push_back(face);
	}
    
	m_nh_box = new Mesh(vertices, faces);
}

NonhierBox::~NonhierBox()
{
	delete m_nh_box;
}

Vector3D NonhierBox::getNormal(const Point3D& p) const
{
    return m_nh_box->getNormal(p);
}

bool NonhierBox::intersect(const Point3D& eye, const Vector3D& ray,  IntersectionPoint &ip, double refl_id)
{
	return m_nh_box->intersect(eye, ray, ip, refl_id);
}

Point2D NonhierBox::get2DTextureMapCoordinates(const Point3D &p) const
{
    return m_nh_box->get2DTextureMapCoordinates(p);
}

Cylinder::Cylinder() :
    m_top_disk(Point3D(0.0, 0.0, 1.0), 1.0, Vector3D(0.0, 0.0, 1.0)),
    m_bottom_disk(Point3D(0.0, 0.0, 0.0), 1.0, Vector3D(0.0, 0.0, -1.0))
{
    
}

Cylinder::~Cylinder()
{
    
}

Vector3D Cylinder::getNormal(const Point3D &p) const
{
    Region r = getRegionForPoint(p);
    Vector3D n;
    switch (r) {
        case CYL_REAR:
            n = m_bottom_disk.getNormal();
            break;
        case CYL_FRONT:
            n = m_top_disk.getNormal();
            break;
        case CYL_BODY:
            n = Vector3D(p[0], p[1], 0.0);
            break;
        default:
            break;
    }
    return n;
}

bool Cylinder::intersect(const Point3D &eye, const Vector3D &ray, IntersectionPoint &ip, double refl_id)
{
    double A = 0;
	double B = 0;
	double C = 0;
    
	for (int i = 0; i < 2; i++) {
		A += ray[i] * ray[i];
		B += 2.0 * eye[i] * ray[i];
		C += eye[i] * eye[i];
	}
	C -= 1.0;
    
    
	double roots[2];
	double t = DBL_MAX;
    
    IntersectionPoint tempIP = ip;
    double ttop = DBL_MAX;
    double tbottom = DBL_MAX;
    bool topHit = m_top_disk.intersect(eye, ray, tempIP, refl_id);
    if (topHit) ttop = tempIP.m_t;
    bool bottomHit = m_bottom_disk.intersect(eye, ray, tempIP, refl_id);
    if (bottomHit) tbottom = tempIP.m_t;
    size_t num = quadraticRoots(A, B, C, roots);
    if (roots[0] < SMALL_EPSILON) roots[0] = DBL_MAX;
    if (roots[1] < SMALL_EPSILON) roots[1] = DBL_MAX;
    if (num) {
        if (check_t(eye, ray, roots[0]) && check_t(eye, ray, roots[1])) {
            t = roots[0] < roots[1] ? roots[0] : roots[1];
        } else if (check_t(eye, ray, roots[0])) {
            t = roots[0];
        } else if (check_t(eye, ray, roots[1])) {
            t = roots[1];
        } else {
            num = 0;
        }
        
    }
    
    bool hit =  (num != 0 && t >= SMALL_EPSILON && t != DBL_MAX ) || topHit || bottomHit;
    
	if (hit) {
        
        if (ttop != DBL_MAX) {
            t = fmin(t, ttop);
        }
        if (tbottom != DBL_MAX) {
            t = fmin(t, tbottom);
        }
		Point3D p = eye + t * ray;
        //        if (p[0] * p[0] + p[1] * p[1] - 100*EPSILON >= p[2] * p[2]) {
        //            return false;
        //        }
        ip.m_t = t;
        ip.m_normal = ip.m_orig_normal = getNormal(p);
        ip.m_normal.normalize();
	}
	return hit;
}

Cylinder::Region Cylinder::getRegionForPoint(const Point3D &p) const
{
    Region r = CYL_BODY;
    if (p[2] > -EPSILON && p[2] < EPSILON) {
        r = CYL_REAR;
    } else if (p[2] > 1.0 - EPSILON && p[2] < 1.0 + EPSILON) {
        r = CYL_FRONT;
    }
    return r;
}

bool Cylinder::check_t(const Point3D &eye, const Vector3D &ray, double t) const
{
    bool onCylinder = true;
    if (t <= SMALL_EPSILON) return false;
    Point3D p = eye + t * ray;
    if (p[0] * p[0] + p[1] * p[1] - EPSILON >= 1.0) {
        onCylinder = false;
    }
    return ((ray[2] < 0) ? t <= -eye[2] / ray[2] : (ray[2] > 0)?t >= -eye[2] / ray[2]:false)
    && ((ray[2] < 0) ? t >= (1.0-eye[2]) / ray[2] : (ray[2] > 0)? t <= (1.0-eye[2]) / (double)ray[2] : false) && onCylinder;
}

Point2D Cylinder::get2DTextureMapCoordinates(const Point3D &p) const
{
    Point2D ret(-1, -1);
    Region r = getRegionForPoint(p);
    switch (r) {
        case CYL_BODY:
        {
            double phi = atan2(-(double)p[1],-(double)p[0]);
            double u = (phi + M_PI) / (2.0 * M_PI);
            if (u > 1.0) u = u - 1.0;
            if (u < 0.0) u = u + 1.0;
            double v = p[2];

            if (v > 1.0) v = v - 1.0;
            if (v < 0.0) v = v + 1.0;
            ret = Point2D(u,v);
        }
            break;
        case CYL_FRONT:
            ret = m_top_disk.get2DTextureMapCoordinates(p);
            break;
        case CYL_REAR:
            ret = m_bottom_disk.get2DTextureMapCoordinates(p);
            break;
        default:
            break;
    }
    
    return ret;
    
    
}

Cone::Cone() :
    m_top_disk(Point3D(0.0, 0.0, 1.0), 1.0, Vector3D(0.0, 0.0, 1.0)),
    m_bottom_disk(Point3D(0.0, 0.0, 0.0), 0.0, Vector3D(0.0, 0.0, -1.0))
{}

Cone::~Cone()
{
    
}

Vector3D Cone::getNormal(const Point3D &p) const
{
    Vector3D n;
    Region r = getRegionForPoint(p);
    switch (r) {
        case CON_BOTTOM:
            n = m_bottom_disk.getNormal();
            break;
        case CON_BODY:
        {
//            Point3D top = Point3D(0,0,1.0);
//            n = Vector3D(p[0], p[1], (double)sqrt(2)/2.0*(p-top).length());
            Vector3D v1 = p - Point3D(0.0, 0.0, 0.0);
            Vector3D v2 = Vector3D(-1.0 * p[1], p[0], 0);
            n = v2.cross(v1);
            break;
        }
        case CON_TOP:
            n = m_top_disk.getNormal();
            break;
        default:
            break;
    }
    return n;
}

Cone::Region Cone::getRegionForPoint(const Point3D &p) const
{
    Region r = CON_BODY;
    if (p[2] > -EPSILON && p[2] < EPSILON) {
        r = CON_BOTTOM;
    } else if (p[2] > 1.0 - EPSILON && p[2] < 1.0 + EPSILON) {
        r = CON_TOP;
    }
    return r;
}

bool Cone::check_t(const Point3D &eye, const Vector3D &ray, double t) const
{
    bool onCone = true;
    if (t <= SMALL_EPSILON) return false;
    Point3D p = eye + t * ray;
    if (p[0] * p[0] + p[1] * p[1] - EPSILON >= p[2] * p[2]) {
        onCone = false;
    }
    return ((ray[2] < 0) ? t <= -eye[2] / ray[2] : (ray[2] > 0)?t >= -eye[2] / ray[2]:false)
    && ((ray[2] < 0) ? t >= (1.0-eye[2]) / ray[2] : (ray[2] > 0)? t <= (1.0-eye[2]) / (double)ray[2] : false) && onCone;
}

bool Cone::intersect(const Point3D &eye, const Vector3D &ray, IntersectionPoint &ip, double refl_id)
{
    
    double A = 0;
	double B = 0;
	double C = 0;
    
    for (int i = 0; i < 3; i++) {
		A += ((i==2)? -1.0 : 1.0) * ray[i] * ray[i];
		B += ((i==2)? -1.0 : 1.0) * 2.0 * eye[i] * ray[i];
		C += ((i==2)? -1.0 : 1.0) * eye[i] * eye[i];
	}
    
    
	double roots[2];
	double t = DBL_MAX;
    
    IntersectionPoint tempIP = ip;
    double ttop = DBL_MAX;
    double tbottom = DBL_MAX;
    bool topHit = m_top_disk.intersect(eye, ray, tempIP, refl_id);
    if (topHit) ttop = tempIP.m_t;
    bool bottomHit = m_bottom_disk.intersect(eye, ray, tempIP, refl_id);
    if (bottomHit) tbottom = tempIP.m_t;
    size_t num = quadraticRoots(A, B, C, roots);
    if (roots[0] < SMALL_EPSILON) roots[0] = DBL_MAX;
    if (roots[1] < SMALL_EPSILON) roots[1] = DBL_MAX;
    if (num) {
        if (check_t(eye, ray, roots[0]) && check_t(eye, ray, roots[1])) {
            t = roots[0] < roots[1] ? roots[0] : roots[1];
        } else if (check_t(eye, ray, roots[0])) {
            t = roots[0];
        } else if (check_t(eye, ray, roots[1])) {
            t = roots[1];
        } else {
            num = 0;
        }
    }

    bool hit =  (num != 0 && t >= SMALL_EPSILON && t != DBL_MAX) || topHit || bottomHit;
    
	if (hit) {

        if (ttop != DBL_MAX) {
            t = fmin(t, ttop);
        }
        if (tbottom != DBL_MAX) {
            t = fmin(t, tbottom);
        }
		Point3D p = eye + t * ray;
        ip.m_t = t;
        ip.m_normal = ip.m_orig_normal = getNormal(p);
        ip.m_normal.normalize();
	}
	return hit;

}

Point2D Cone::get2DTextureMapCoordinates(const Point3D &p) const
{
    Point2D ret(-1, -1);
    Region r = getRegionForPoint(p);
    switch (r) {
        case CON_BODY:
        {
            double phi = atan2(-(double)p[1],-(double)p[0]);
            double u = (phi + M_PI) / ( 2.0 * M_PI);
            if (u > 1.0) u = u - 1.0;
            if (u < 0.0) u = u + 1.0;
            double v = p[2];
            
            if (v > 1.0) v = v - 1.0;
            if (v < 0.0) v = v + 1.0;
            ret = Point2D(u,v);
        }
            break;
        case CON_TOP:
            ret = m_top_disk.get2DTextureMapCoordinates(p);
            break;
        case CON_BOTTOM:
            ret = m_bottom_disk.get2DTextureMapCoordinates(p);
            break;
        default:
            break;
    }
    
    return ret;
}

Torus::Torus(double inner_radius, double outer_radius) :
    m_r(inner_radius),
    m_R(outer_radius)
{}

Torus::~Torus()
{
    
}

Vector3D Torus::getNormal(const Point3D &p) const
{
    double x = p[0];
    double y = p[1];
    double z = p[2];
    double common = x*x + y*y + z*z - m_r*m_r - m_R*m_R;
    return Vector3D(
        4.0 * x * common,
        4.0 * y * common,
        4.0 * z * common + 8.0 * m_R * m_R * z
    );
}

bool Torus::intersect(const Point3D &eye, const Vector3D &ray, IntersectionPoint &ip, double refl_id)
{
    Vector3D eyeVec = eye - Point3D();
    
    double a = ray.dot(ray);
    double b = 2.0 * eyeVec.dot(ray);
    double c = eyeVec.dot(eyeVec) - m_r*m_r - m_R*m_R;
    
    double A4 = a * a;
    double A3 = 2.0 * a * b;
    double A2 = b * b + 2.0 * a * c + 4.0 * m_R * m_R * ray[2] * ray[2];
    double A1 = 2.0 * b * c + 8.0 * m_R * m_R * eye[2] * ray[2];
    double A0 = c * c + 4.0 * m_R * m_R * (eye[2] * eye[2] - m_r * m_r);
    double A = A3 / A4;
    double B = A2 / A4;
    double C = A1 / A4;
    double D = A0 / A4;
    double roots[4];
    size_t num = quarticRoots(A, B, C, D, roots);
    double minRoot = DBL_MAX;
    
    for (unsigned int i = 0; i < num; ++i) {
        if (roots[i] < SMALL_EPSILON) roots[i] = DBL_MAX;
        minRoot = fmin(minRoot, roots[i]);
    }
    bool hit = minRoot != DBL_MAX && minRoot >= SMALL_EPSILON && minRoot != DBL_MAX;
    if (hit) { // hit
        Point3D p = eye + minRoot * ray;
        // ip.m_point = p;
        ip.m_t = minRoot;
        ip.m_normal = ip.m_orig_normal = getNormal(p);
        ip.m_normal.normalize();
    }
    return hit;

}

Point2D Torus::get2DTextureMapCoordinates(const Point3D &p) const
{
    double phi = atan2(-p[1], -p[0]);
    double u = (phi + M_PI)/ (2.0 * M_PI);
    if (u > 1.0) u = u - 1.0;
    if (u < 0.0) u = u + 1.0;
    double theta = asin(p[2]/m_r);
    double v = (theta + M_PI_2) / (M_PI);
    if (v > 1.0) v = v - 1.0;
    if (v < 0.0) v = v + 1.0;
    return Point2D(u, v);
}



CornellBox::CornellBox(double width, double height)
{
    this->m_width = width;
    this->m_height = height;
    this->m_depth = (double)(width + height) / 2.0;
    std::vector<Point3D> vertices;
    Point3D pos(-(m_width/2.0), -(m_height/2.0), 0);
	vertices.push_back(Point3D(pos[0], pos[1], pos[2]));
	vertices.push_back(Point3D(pos[0] + m_width, pos[1], pos[2]));
	vertices.push_back(Point3D(pos[0] + m_width, pos[1] + m_height, pos[2]));
	vertices.push_back(Point3D(pos[0], pos[1] + m_height, pos[2]));
	vertices.push_back(Point3D(pos[0], pos[1], pos[2] + m_depth));
	vertices.push_back(Point3D(pos[0] + m_width, pos[1], pos[2] + m_depth));
	vertices.push_back(Point3D(pos[0] + m_width, pos[1] + m_height, pos[2] + m_depth));
	vertices.push_back(Point3D(pos[0], pos[1] + m_height, pos[2] + m_depth));
    
	std::vector<std::vector<int> > faces;
	int f[4][4] = {
		{4, 5, 1, 0}, 	 // bottom
		{1, 5, 6, 2}, 	 // right
		{4, 0, 3, 7}, 	 // left
//		{3, 2, 6, 7}, 	 // top
		{0, 1, 2, 3}	 // rear
	};
	for (int i = 0; i < 4; ++i) {
		std::vector<int> face;
		for (int j = 0; j < 4; ++j) {
			face.push_back(f[i][j]);
		}
		faces.push_back(face);
	}
    
	m_cornell_box = new Mesh(vertices, faces);
}

CornellBox::~CornellBox()
{
    delete m_cornell_box;
}

Vector3D CornellBox::getNormal(const Point3D &p) const
{
    return m_cornell_box->getNormal(p);
}

bool CornellBox::intersect(const Point3D &eye, const Vector3D &ray, IntersectionPoint &ip, double refl_id)
{
    return m_cornell_box->intersect(eye, ray, ip, refl_id);
}

Point2D CornellBox::get2DTextureMapCoordinates(const Point3D &p) const
{
    return m_cornell_box->get2DTextureMapCoordinates(p);
}
