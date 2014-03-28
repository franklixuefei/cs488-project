#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "intersection_point.hpp"
#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "shape2d.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual Vector3D getNormal(const Point3D &p)const=0;
  virtual bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id)=0;
    virtual Point2D get2DTextureMapCoordinates(const Point3D &p) const=0;
};

class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces,
       bool isBoundingBox = false);
  virtual ~Mesh();
  Vector3D getNormal(const Point3D &p) const;
  typedef std::vector<int> Face;
  bool intersect(const Point3D& eye, const Vector3D& ray,  IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  std::vector<Polygon*> m_polygons;
  Mesh *m_bounding_box;
  Point3D m_bounding_box_min;
  Point3D m_bounding_box_max;
private:
  bool within_bounding_box(const Point3D &p);
  Polygon* getPolygonForPoint(const Point3D& p) const;
  void findExtremePoints( Point3D &min, Point3D &max);
  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  Vector3D getNormal(const Point3D &p) const;
  bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
  Primitive *m_basic_sphere;
};

class Cube : public Primitive {
public:
  Cube();
  virtual ~Cube();
  Vector3D getNormal(const Point3D &p) const;
  bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
  Primitive *m_basic_cube;
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  Vector3D getNormal(const Point3D &p) const;
  bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
  Point3D m_pos;
  double m_radius;
private:
  bool isWithinBound(const Point3D &eye, const Vector3D &ray);
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size);
  virtual ~NonhierBox();
  Vector3D getNormal(const Point3D &p) const;
  bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
  Point3D m_pos;
  double m_size;
  Mesh *m_nh_box;
};

class Cylinder : public Primitive {
public:
    Cylinder();
    virtual ~Cylinder();
    Vector3D getNormal(const Point3D &p) const;
    bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
    enum Region {
        CYL_REAR,
        CYL_BODY,
        CYL_FRONT
    };
    Region getRegionForPoint(const Point3D &p) const;
    bool check_t(const Point3D &eye, const Vector3D &ray, double t) const;
private:
    Disk m_top_disk;
    Disk m_bottom_disk;
    
};

class Cone : public Primitive {
public:
    Cone();
    virtual ~Cone();
    Vector3D getNormal(const Point3D &p) const;
    bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
    enum Region {
        CON_BOTTOM,
        CON_BODY,
        CON_TOP
    };
    Region getRegionForPoint(const Point3D &p) const;
    bool check_t(const Point3D &eye, const Vector3D &ray, double t) const;
private:
    Disk m_bottom_disk;
    Disk m_top_disk;
};

class Torus : public Primitive {
public:
    Torus(double inner_radius, double outer_radius);
    virtual ~Torus();
    Vector3D getNormal(const Point3D &p) const;
    bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
    double m_r;
    double m_R;
};

class CornellBox : public Primitive {
public:
    CornellBox(double width, double height);
    virtual ~CornellBox();
    Vector3D getNormal(const Point3D &p) const;
    bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint &ip, double refl_id);
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
    double m_width;
    double m_height;
    double m_depth;
    Mesh *m_cornell_box;
};

#endif
