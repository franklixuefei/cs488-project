#include "light.hpp"
#include <iostream>

Light::Light()
  : m_colour(0.0, 0.0, 0.0),
    m_position(0.0, 0.0, 0.0)
{
  m_falloff[0] = 1.0;
  m_falloff[1] = 0.0;
  m_falloff[2] = 0.0;
}

Light::Light(
             Colour colour,
             const Point3D &position,
             const Vector3D &attenuation,
             size_t num_photons,
             double photon_power) :
    m_colour(colour),
    m_position(position),
    m_num_photons(num_photons),
    m_photon_power(photon_power)
{
    m_falloff[0] = attenuation[0];
    m_falloff[1] = attenuation[1];
    m_falloff[2] = attenuation[2];
}

Light::~Light()
{}

Colour Light::getColour() const
{
    return m_colour;
}

void Light::setColour(const Colour &colour)
{
    m_colour = colour;
}

Point3D Light::getPosition() const
{
    return m_position;
}

void Light::setPosition(const Point3D &position)
{
    m_position = position;
}

Vector3D Light::getAttenuation() const
{
    return Vector3D(m_falloff[0], m_falloff[1], m_falloff[2]);
}

void Light::setAttenuation(const Vector3D &attenuation)
{
    m_falloff[0] = attenuation[0];
    m_falloff[1] = attenuation[1];
    m_falloff[2] = attenuation[2];
}

size_t Light::getNumPhotons() const
{
    return m_num_photons;
}

double Light::getPower() const
{
    return m_photon_power;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
    out << "colour: " << l.m_colour << ",\n\tposition: " << l.m_position << ",\n\tnum photons: " << l.m_num_photons << ",\n\tphoton power: " << l.m_photon_power << ",\n\tfalloff: {" << l.m_falloff[0] << ", " << l.m_falloff[1] << ", " << l.m_falloff[2] << "}";
    return out;
}

PointLight::PointLight() : Light() {}

PointLight::PointLight(
                       Colour colour,
                       const Point3D &position,
                       const Vector3D &attenuation,
                       size_t num_photons,
                       double photon_power) :
    Light(colour, position, attenuation, num_photons, photon_power)
{}

PointLight::~PointLight()
{}

SquareLight::SquareLight() :
    Light(),
    m_size(1.5)
{}

SquareLight::SquareLight(
                         Colour colour,
                         const Point3D &position,
                         const Vector3D &attenuation,
                         size_t num_photons,
                         double photon_power,
                         double size) :
    Light(colour, position, attenuation, num_photons, photon_power),
    m_size(size)
{}

SquareLight::~SquareLight()
{}

double SquareLight::getSize() const
{
    return m_size;
}

void SquareLight::setSize(double size)
{
    m_size = size;
}

