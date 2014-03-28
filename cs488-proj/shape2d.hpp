//
//  shape2d.h
//  cs488-proj
//
//  Created by Frank Li on 2014-03-20.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#ifndef __cs488_proj__shape2d__
#define __cs488_proj__shape2d__
#include "algebra.hpp"
#include <vector>
#include "intersection_point.hpp"
#include <float.h>
#include <iostream>
using namespace std;

struct Plane {
    Plane() {}
    Plane(const Vector3D& n, const Point3D& p, const Vector3D &up = Vector3D(0.0, 1.0, 0.0)) :
    m_plane_normal(n),
    m_plane_orig_normal(n),
    m_sample_point(p),
    m_up_vec(up),
    m_right_vec(up.cross(n))
    {
        m_plane_normal.normalize();
        m_up_vec.normalize();
        m_right_vec.normalize();
    }
    
    double calcDistRatio(const Point3D& eye, const Vector3D& ray) const
    {
        Vector3D v = m_sample_point - eye;
        double rayProj = ray.dot(m_plane_normal);
        
        return v.dot(m_plane_normal) / rayProj;
    }
    
    void set_up_vec(const Vector3D &up)
    {
        m_up_vec = up;
        m_right_vec = up.cross(m_plane_orig_normal);
        m_up_vec.normalize();
        m_right_vec.normalize();
    }
    
    Vector3D m_plane_normal;
    Vector3D m_plane_orig_normal; // pre-normalized normal
    Point3D m_sample_point;
    Vector3D m_right_vec;
    Vector3D m_up_vec;
};

class Shape2D {
public:
    Shape2D() {}
    virtual ~Shape2D()=0;
    virtual bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id)=0;
    virtual Vector3D getNormal() const=0;
    virtual Point2D get2DTextureMapCoordinates(const Point3D &p) const=0;
};


class Polygon : public Shape2D {
public:
 	
	Polygon() {}
	Polygon(std::vector<Point3D>& vertices, const Vector3D& normal);
	virtual ~Polygon();
    
	bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id);
	bool isWithinPolygon(const Point3D& p);
	bool isPointOnPlane(const Point3D& p);
	Vector3D getNormal() const;
    
//	void transform(const Matrix4x4& m);
	const std::vector<Point3D> & get_vertices();
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
	bool isPointWithinPolygonForEdge(const Point3D& p, const Point3D& p1, const Point3D& p2);
    
	std::vector<Point3D> m_vertices;
	Plane m_plane;
};

class Disk : public Shape2D {
public:
    Disk() {}
    Disk(const Point3D &center, double radius, const Vector3D &normal);
    virtual ~Disk();
	bool intersect(const Point3D& eye, const Vector3D& ray, IntersectionPoint& ip, double refl_id);
    Vector3D getNormal() const;
    Point3D getCenter() const;
    Point2D get2DTextureMapCoordinates(const Point3D &p) const;
private:
    Plane m_plane;
    Point3D m_center;
    double m_radius;
    
};

#endif /* defined(__cs488_proj__shape2d__) */
