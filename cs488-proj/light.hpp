#ifndef CS488_LIGHT_HPP
#define CS488_LIGHT_HPP

#include "algebra.hpp"
#include <iosfwd>

// Represents a simple point light.
class Light {
public:
    Light();
    Light(
          Colour colour,
          const Point3D &position,
          const Vector3D &attenuation,
          size_t num_photons,
          double photon_power,
          double power_bias);
    virtual ~Light()=0;
    Colour getColour() const;
    void setColour(const Colour &colour);
    Point3D getPosition() const;
    void setPosition(const Point3D &position);
    Vector3D getAttenuation() const;
    void setAttenuation(const Vector3D &attenuation);
    size_t getNumPhotons() const;
    double getPower() const;
    double getBias() const;
    friend std::ostream& operator<<(std::ostream& out, const Light& l);
    
protected:
    Colour m_colour;
    Point3D m_position;
    size_t m_num_photons;
    double m_photon_power;
    double m_power_bias;
    double m_falloff[3];
};

class PointLight : public Light {
public:
    PointLight();
    PointLight(
               Colour colour,
               const Point3D &position,
               const Vector3D &attenuation,
               size_t num_photons,
               double photon_power,
               double power_bias);
    virtual ~PointLight();
    
};

class SquareLight : public Light {
public:
    SquareLight();
    SquareLight(
                Colour colour,
                const Point3D &position,
                const Vector3D &attenuation,
                size_t num_photons,
                double photon_power,
                double power_bias,
                double size);
    virtual ~SquareLight();
    double getSize() const;
    void setSize(double size);
protected:
    double m_size;
};



#endif
