#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "intersection_point.hpp"
#include "light.hpp"

class Renderer;
class IntersectionPoint;
class GeometryNode;
class SceneNode {
public:
    SceneNode(const std::string& name);
    virtual ~SceneNode();
    
    const Matrix4x4& get_transform() const { return m_trans; }
    const Matrix4x4& get_inverse() const { return m_invtrans; }
    
    void set_transform(const Matrix4x4& m)
    {
        m_trans = m;
        m_invtrans = m.invert();
    }
    
    void set_transform(const Matrix4x4& m, const Matrix4x4& i)
    {
        m_trans = m;
        m_invtrans = i;
    }
    
    void add_child(SceneNode* child)
    {
        m_children.push_back(child);
    }
    
    void remove_child(SceneNode* child)
    {
        m_children.remove(child);
    }
    
    bool intersect(
                   const Point3D& eye,
                   const Vector3D& ray,
                   Colour& c,
                   const std::list<Light*>& lights,
                   const Colour &ambient,
                   Renderer *renderer,
                   double refl_id,
                   IntersectionPoint &ip);
    
    virtual bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id) const;
    
    // virtual bool intersect(const Point3D& eye, const Vector3D& ray, double &t);
    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3D& amount);
    void translate(const Vector3D& amount);
    
    // Returns true if and only if this node is a JointNode
    // virtual bool is_joint() const;
    std::string getName()const
    {
        return m_name;
    }
    
    
protected:
    static Renderer *s_renderer;
    // Useful for picking
    int m_id;
    std::string m_name;
    
    // Transformations
    Matrix4x4 m_trans;
    Matrix4x4 m_invtrans;
    
    Matrix4x4 m_translation_trans;
    Matrix4x4 m_rotation_x_trans;
    Matrix4x4 m_rotation_y_trans;
    Matrix4x4 m_rotation_z_trans;
    Matrix4x4 m_scaling_trans;
    
    // Hierarchy
    typedef std::list<SceneNode*> ChildList;
    ChildList m_children;
    
};


class GeometryNode : public SceneNode {
public:
    GeometryNode(const std::string& name,
                 Primitive* primitive);
    virtual ~GeometryNode();
    
    const PhongMaterial* get_material() const;
    PhongMaterial* get_material();
    
    void set_material(PhongMaterial* material)
    {
        m_material = material;
    }
    
    const Primitive* get_primitive() const;
    Primitive* get_primitive();
    
    bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id) const;
    
    Colour getColourForPoint(
                             const Point3D &eye,
                             const IntersectionPoint& ip,
                             const std::list<Light*>& lights,
                             const Colour &ambient,
                             double refl_id,
                             int recursive_depth);
    
protected:
    PhongMaterial* m_material;
    Primitive* m_primitive;
private:
    double calcFresnelReflectance(const Vector3D& view_dir,
                                  const Vector3D& normal,
                                  const Point3D& start,
                                  const std::list<Light*>& lights,
                                  const Colour &ambient,
                                  double refl_id);
    
    Colour refractionContribution(
                                  const Vector3D& view_dir,
                                  const Vector3D& normal,
                                  const Point3D& start,
                                  const std::list<Light*>& lights,
                                  const Colour &ambient,
                                  double refl_id,
                                  double &reflectance,
                                  int recursive_depth) const;
    
    Colour reflectionContribution(
                                  const Vector3D& view_dir,
                                  const Vector3D& normal,
                                  const Point3D& start,
                                  const std::list<Light*>& lights,
                                  const Colour &ambient,
                                  double refl_id,
                                  int recursive_depth) const;
    Colour lightsContribution(
                              const IntersectionPoint& ip, 
                              const std::list<Light*>& lights,
                              const Colour &ambient,
                              const Vector3D &view_dir);
};




#endif
