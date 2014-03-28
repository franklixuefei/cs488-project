#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include "light.hpp"
#include <list>
#include "intersection_point.hpp"
#include "image.hpp"
using namespace std;

class Primitive;

class PhongMaterial {
public:
    PhongMaterial(const Colour& ks, double shininess, double refractive_index);
    virtual ~PhongMaterial();
    
    virtual Colour getDiffuse(Primitive *primitive, const Point3D &p) const=0;
    Colour getSpecular() const;
    Colour getShininess() const;
    
    Colour getColour(
                     const Vector3D& normal,
                     const Vector3D& view_dir,
                     const std::list<Light*>& lights,
                     const Colour& ambient,
                     const IntersectionPoint &ip,
                     Primitive *primitive) const;
    double getRefractiveIndex() const;
    
protected:
    Colour m_ks;
    double m_shininess;
    double m_test_reflectance; // for hard-coded testing.
    double m_refractive_index; // n (glass = 1.5, water = 1.33, air = 1, non-trans = +inf)
};


class BasicPhongMaterial : public PhongMaterial {
public:
    BasicPhongMaterial(const Colour &kd, const Colour &ks, double shininess, double refractive_index);
    virtual ~BasicPhongMaterial();
    Colour getDiffuse(Primitive *primitive, const Point3D &p) const;
protected:
    Colour m_kd;
};


class ImageTextureMaterial : public PhongMaterial {
public:
    ImageTextureMaterial(const string &filename, const Colour &ks, double shininess, double refractive_index);
    virtual ~ImageTextureMaterial();
    Colour getDiffuse(Primitive *primitive, const Point3D &p) const;
protected:
    Image *m_texture_map;
protected:
    bool getMapCoords(Primitive *primitive, const Point3D &p, Point2D& mapCoords) const;
};


#endif
