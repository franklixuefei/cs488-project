#include "material.hpp"
#include "primitive.hpp"

PhongMaterial::PhongMaterial(const Colour& ks, double shininess, double refractive_index)
: m_ks(ks), m_shininess(shininess), m_refractive_index(refractive_index)
{
}

PhongMaterial::~PhongMaterial()
{
}

Colour PhongMaterial::getSpecular() const
{
	return m_ks;
}
Colour PhongMaterial::getShininess() const
{
	return m_shininess;
}

Colour PhongMaterial::getColour(
                                const Vector3D& normal,
                                const Vector3D& view_dir,
                                const std::list<Light*>& lights,
                                const Colour& ambient,
                                const IntersectionPoint &ip,
                                Primitive *primitive) const
{
    
	Vector3D view = view_dir;
	view.normalize();
    
    Colour kd = getDiffuse(primitive, ip.m_orig_point);
    
    Colour c = kd * ambient;
    
    for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
        Vector3D light_dir = (*it)->position - ip.m_point; // Note this needs to point towards the light.
        
        double dist = light_dir.length();
        light_dir.normalize();
        Vector3D r = -light_dir + 2 * (light_dir.dot(normal)) * normal;
        
        Colour contribution = (kd + m_ks * ( pow(r.dot(view), m_shininess) / normal.dot(light_dir) )) *
        (*it)->colour * light_dir.dot(normal) *
        (1 / ((*it)->falloff[0] + (*it)->falloff[1] * dist +
              (*it)->falloff[2] * dist * dist));
        if (contribution.R() >= 0 && contribution.G() >= 0 && contribution.B() >= 0) {
            c = c + contribution;
        }
    }
    
    return c;
}

double PhongMaterial::getRefractiveIndex() const
{
    return m_refractive_index;
}




BasicPhongMaterial::BasicPhongMaterial(const Colour &kd, const Colour &ks, double shininess, double refractive_index) :
PhongMaterial(ks, shininess, refractive_index),
m_kd(kd)
{
    
}

BasicPhongMaterial::~BasicPhongMaterial()
{
    
}

Colour BasicPhongMaterial::getDiffuse(Primitive *primitive, const Point3D &p) const
{
    return m_kd;
}


ImageTextureMaterial::ImageTextureMaterial(const string &filename, const Colour &ks, double shininess, double refractive_index) :
PhongMaterial(ks, shininess, refractive_index),
m_texture_map(new Image())
{
    if (!m_texture_map->loadPng(filename)) {
        cerr << "texture map " << filename << " not found." << endl;
        exit(1);
    }
}

ImageTextureMaterial::~ImageTextureMaterial()
{
    delete m_texture_map;
}

Colour ImageTextureMaterial::getDiffuse(Primitive *primitive, const Point3D &p) const
{
    Point2D mapCoords(-1,-1);
    if (!getMapCoords(primitive, p, mapCoords)) {
        cerr << "texture map coords out of bound"  << endl;
        return Colour(1,0,0);
    }
    // bilinear interpolation
    int x0 = int(mapCoords[0]);
    int y0 = int(mapCoords[1]);
    double dx = mapCoords[0] - x0;
    double dy = mapCoords[1] - y0;
    double omdx = 1.0 - dx;
    double omdy = 1.0 - dy;
    Colour c00, c01, c10, c11;
#warning using supersamping to make the texture smooth
    if (x0 < m_texture_map->width() && y0 < m_texture_map->height()) {
        c00 = Colour(
                     (*m_texture_map)(x0, y0, 0),
                     (*m_texture_map)(x0, y0, 1),
                     (*m_texture_map)(x0, y0, 2)
                     );
    }
    if (x0 < m_texture_map->width() && y0+1 < m_texture_map->height()) {
        c01 = Colour(
                     (*m_texture_map)(x0, y0+1, 0),
                     (*m_texture_map)(x0, y0+1, 1),
                     (*m_texture_map)(x0, y0+1, 2)
                     );
    }
    if (x0+1 < m_texture_map->width() && y0 < m_texture_map->height()) {
        c10 = Colour(
                     (*m_texture_map)(x0+1, y0, 0),
                     (*m_texture_map)(x0+1, y0, 1),
                     (*m_texture_map)(x0+1, y0, 2)
                     );
    }
    if (x0+1 < m_texture_map->width() && y0+1 < m_texture_map->height()) {
        c11 = Colour(
                     (*m_texture_map)(x0+1, y0+1, 0),
                     (*m_texture_map)(x0+1, y0+1, 1),
                     (*m_texture_map)(x0+1, y0+1, 2)
                     );
    }
    Colour ret =    omdx * omdy * c00 +
    omdx * dy * c01 +
    dx * omdy * c10 +
    dx * dy * c11;
    return ret;
}

bool ImageTextureMaterial::getMapCoords(Primitive *primitive, const Point3D &p, Point2D& mapCoords) const
{
    Point2D uvCoords = primitive->get2DTextureMapCoordinates(p);
    int texture_width = m_texture_map->width();
    int texture_height = m_texture_map->height();
    mapCoords = Point2D(
                        uvCoords[0] * (double)((double)texture_width - 1.0),
                        uvCoords[1] * (double)((double)texture_height - 1.0)
                        );
    if (mapCoords[0] < -EPSILON || mapCoords[0] > (double)texture_width + EPSILON ||
        mapCoords[1] < -EPSILON || mapCoords[1] > (double)texture_height + EPSILON) {
        return false;
    }
    return true;
    
}



