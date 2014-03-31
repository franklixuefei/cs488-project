#include "scene.hpp"
#include <iostream>
#include "transform.hpp"
#include "intersection_point.hpp"
#include <vector>
#include <float.h>
#include <climits>
#include "renderer.hpp"
#include "formulas.hpp"
#include <cassert>

SceneNode::SceneNode(const std::string& name)
: m_name(name)
{
}

SceneNode::~SceneNode()
{
}

Renderer *SceneNode::s_renderer = NULL;

bool SceneNode::intersect(
                          const Point3D& eye,
                          const Vector3D& ray,
                          Colour& c,
                          const std::list<Light*>& lights,
                          const Colour &ambient,
                          Renderer *renderer,
                          double refl_id,
                          IntersectionPoint &ip)
{
    if (!s_renderer) {
        s_renderer = renderer;
    }
    if (intersect(eye, ray, ip, refl_id)) {
        
        c = ip.m_owner->getColourForPoint(eye, ip, lights, ambient, refl_id);
        return true;
    }
    return false;
}

bool SceneNode::intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id) const
{
    const Point3D world_eye = m_invtrans * eye;
    const Vector3D world_ray = m_invtrans * ray;
    IntersectionPoint minIP;
    minIP.m_t = DBL_MAX;
    for (ChildList::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
        if ((*it)->intersect(world_eye, world_ray, ip, refl_id)) {
            if (ip < minIP) {
                minIP = ip;
            }
        }
    }
    ip = minIP;
    ip.m_normal = ip.m_orig_normal = m_invtrans.transpose() * ip.m_orig_normal;
    ip.m_normal.normalize();
    ip.m_point = eye +  ip.m_t * ray;
    return ip.m_t != DBL_MAX;
}


void SceneNode::rotate(char axis, double angle)
{
    angle = angle * M_PI / 180.0f;
    set_transform(m_trans * rotation(angle, axis) );
}

void SceneNode::scale(const Vector3D& amount)
{
    set_transform(m_trans * scaling(amount));
}

void SceneNode::translate(const Vector3D& amount)
{
    set_transform(m_trans * translation(amount));
}


GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
: SceneNode(name),
m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
    
}

const PhongMaterial* GeometryNode::get_material() const
{
    return m_material;
}

PhongMaterial* GeometryNode::get_material()
{
    return m_material;
}

const Primitive* GeometryNode::get_primitive() const
{
    return m_primitive;
}

Primitive* GeometryNode::get_primitive()
{
    return m_primitive;
}

bool GeometryNode::intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id) const
{
    const Point3D world_eye = m_invtrans * eye;
    const Vector3D world_ray = m_invtrans * ray;
    ip.m_owner = (GeometryNode*)this;
    if (m_primitive->intersect(world_eye, world_ray, ip, refl_id)) {
        ip.m_orig_point = world_eye + ip.m_t*world_ray;
        ip.m_normal = ip.m_orig_normal = m_invtrans.transpose() * ip.m_orig_normal;
        ip.m_normal.normalize();
        return true;
    }
    return false;
}

Colour GeometryNode::getColourForPoint(
                                       const Point3D &eye,
                                       const IntersectionPoint& ip,
                                       const std::list<Light*>& lights,
                                       const Colour &ambient,
                                       double refl_id,
                                       int recursive_depth)
{
    
    Colour c(0,0,0);
    
    Vector3D view_dir = ip.m_point - eye; // pointing to incident point
    view_dir.normalize();
#warning hard-coded reflectance.
    double reflectance = 1.0;
    if (ip.m_owner->get_material()->getRefractiveIndex() != DBL_MAX) {
        Colour crefr = refractionContribution(view_dir, ip.m_orig_normal, ip.m_point, lights, ambient, refl_id, reflectance, recursive_depth);
        c = c + (1.0 - reflectance) * crefr;
    }
    c = c + ip.m_owner->get_material()->getSpecular() * reflectance * reflectionContribution(view_dir, ip.m_orig_normal, ip.m_point, lights, ambient, refl_id, recursive_depth);
    
    c = c + lightsContribution(ip, lights, ambient, view_dir);
    return c;
}

/*
 
 
 
 Vec x=r.o+r.d*t, n=(x-obj.p).norm(), nl=n.dot(r.d)<0?n:n*-1, f=obj.c;
 double p = f.x>f.y && f.x>f.z ? f.x : f.y>f.z ? f.y : f.z; // max refl
 
 Ray reflRay(x, r.d-n*2*n.dot(r.d));     // Ideal dielectric REFRACTION
 bool into = n.dot(nl)>0;                // Ray from outside going in?
 double nc=1, nt=1.5, nnt=into?nc/nt:nt/nc, ddn=r.d.dot(nl), cos2t;
 if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0)    // Total internal reflection
 return obj.e + f.mult(radiance(reflRay,depth,Xi));
 Vec tdir = (r.d*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
 double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
 double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
 return obj.e + f.mult(depth>2 ? (erand48(Xi)<P ?   // Russian roulette
 radiance(reflRay,depth,Xi)*RP:radiance(Ray(x,tdir),depth,Xi)*TP) :
 radiance(reflRay,depth,Xi)*Re+radiance(Ray(x,tdir),depth,Xi)*Tr);
 */

Colour GeometryNode::refractionContribution(
                                            const Vector3D& view_dir,
                                            const Vector3D& normal,
                                            const Point3D& start,
                                            const std::list<Light*>& lights,
                                            const Colour &ambient,
                                            double refl_id,
                                            double &reflectance,
                                            int recursive_depth) const
{
    double hitMatReflID = m_material->getRefractiveIndex();
#warning ray is hitting out of the object (to the air), assuming no two intact objects.
    Vector3D t_dir;
    double na, nb;
    bool into = (hitMatReflID != refl_id);
    if ((normal.dot(view_dir) < 0)) { // ray going in
//        assert(hitMatReflID != refl_id);
        na = refl_id;
        nb = hitMatReflID;
    } else {
//        assert(hitMatReflID == refl_id);
        na = hitMatReflID;
        nb = 1.0;
    }
    double nnt = DBL_MAX, ddn = DBL_MAX;
    Vector3D n = normal;
    n.normalize();
    bool canRefract = Formulas::SnellRefraction(view_dir, normal, na, nb, t_dir, nnt, ddn);
    t_dir.normalize();
// calculate reflectance using Fresnel's Formula
    reflectance = Formulas::schlickApproxFresnelReflectance(1.0, hitMatReflID, (into ? -1.0 * ddn : t_dir.dot(n)));
    if (!canRefract) { // total internal reflection);
        return reflectance * reflectionContribution(view_dir, normal, start, lights, ambient, refl_id, recursive_depth);
    }

    IntersectionPoint ip;
    bool hit = s_renderer->get_root()->intersect(start, t_dir, ip, into? hitMatReflID : 1.0);
    if (hit) {
        return ip.m_owner->getColourForPoint(start, ip, lights, ambient, (into?hitMatReflID:1.0), recursive_depth);
    }
    return Colour(0,0,0);
}


Colour GeometryNode::reflectionContribution(const Vector3D& view_dir, // incoming ray dir
                                            const Vector3D& normal,
                                            const Point3D& start,
                                            const std::list<Light*>& lights,
                                            const Colour &ambient,
                                            double refl_id,
                                            int recursive_depth) const
{
    Vector3D n = normal;
    n.normalize();
    if (recursive_depth >= 0) {
        Vector3D reflected_dir = Formulas::perfectReflection(n, view_dir);
        IntersectionPoint ip;
        
        if (SceneNode::s_renderer->get_root()->intersect(start, reflected_dir, ip, refl_id)) {
            return ip.m_owner->getColourForPoint(start, ip, lights, ambient, refl_id, recursive_depth-1);
        }
        return Colour(0,0,0);
    }
    return Colour(0,0,0);
}



Colour GeometryNode::lightsContribution(
                                        const IntersectionPoint& ip,
                                        const std::list<Light*>& lights,
                                        const Colour &ambient,
                                        const Vector3D &view_dir)
{

    std::list<Light*> visible_lights;
    for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
        PointLight *pl = dynamic_cast<PointLight*>(*it);
        SquareLight *sl = dynamic_cast<SquareLight*>(*it);
        if (pl) {
            
        } else if (sl) {
            #warning TODO: implement soft shadow here!!!
        }
        Vector3D light_dir = (*it)->getPosition() - ip.m_point;
        double lightDistSq = light_dir.length2();
        light_dir.normalize();
        IntersectionPoint temp;
#warning NOTE: if refl_id is DBL_MAX, then either this is an opaque object or we are tracing towards lights, or this is a bounding box.
        bool ObjectInTheWay = SceneNode::s_renderer->get_root()->intersect(ip.m_point, light_dir, temp, DBL_MAX);
        if (!ObjectInTheWay || (temp.m_point - ip.m_point).length2() - lightDistSq > SMALL_EPSILON) { // this light is visible.
            visible_lights.push_back(*it);
        }
        
    }
    const string &render_mode = s_renderer->getRenderMode();
    Colour c;
    if (render_mode == "photon map") { // only render photon map
        double irrad[3];
        s_renderer->getPhotonMap()->irradiance_estimate(irrad, (ip.m_point-Point3D()).getRaw(), (ip.m_normal-Vector3D()).getRaw(), s_renderer->getPhotonSearchRadius(), (int)s_renderer->getNumSearchPhotons());
        c = Colour(irrad[0], irrad[1], irrad[2]);
    } else if (render_mode == "ray tracing") { // only render ray tracing
        c = m_material->getColour(view_dir, visible_lights, ambient, ip, ip.m_owner->get_primitive());
    } else { // "all", render both photon map and ray tracing
        double irrad[3];
        s_renderer->getPhotonMap()->irradiance_estimate(irrad, (ip.m_point-Point3D()).getRaw(), (ip.m_normal-Vector3D()).getRaw(), s_renderer->getPhotonSearchRadius(), (int)s_renderer->getNumSearchPhotons());
        c = Colour(irrad[0], irrad[1], irrad[2]) + m_material->getColour(view_dir, visible_lights, ambient, ip, ip.m_owner->get_primitive());
    }
    return c;
}