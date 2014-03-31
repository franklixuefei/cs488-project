//
//  shape2d.cpp
//  cs488-proj
//
//  Created by Frank Li on 2014-03-20.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#include "shape2d.hpp"
#include <cstdlib>
#include "algebra.hpp"
#include <cstdlib>
#include "scene.hpp"
using namespace std;

Shape2D::~Shape2D()
{
    
}

Polygon::Polygon(std::vector<Point3D>& vertices, const Vector3D& normal) :
    Shape2D(),
    m_vertices(vertices),
    m_plane(normal, vertices.front())
{
    Point3D p1 = m_vertices.back();
    Point3D p2 = m_vertices.front();
    m_plane.set_up_vec(p2-p1);
}

Polygon::~Polygon()
{
    
}

bool Polygon::isPointWithinPolygonForEdge(
                                          const Point3D& p,
                                          const Point3D& p1,
                                          const Point3D& p2)
{
    Vector3D v = p1 - p2;
    Vector3D edgeNormal = v.cross(m_plane.m_plane_normal);
    
    return edgeNormal.dot(p - p1) < SMALL_EPSILON;
}

bool Polygon::intersect(
                        const Point3D& eye,
                        const Vector3D& ray,
                        IntersectionPoint& ip,
                        double refl_id)
{
    double t = m_plane.calcDistRatio(eye, ray);
#warning changed from t<0 to t < SMALL_EPSILON
    if (t < SMALL_EPSILON) return false;
    Point3D p = eye + t * ray;
    // inside of an object (refraction), assuming all materials have different refl index
    bool false_ray_check = (refl_id != DBL_MAX && ip.m_owner->get_material()->getRefractiveIndex() == refl_id) ?
    ray.dot(m_plane.m_plane_normal) < -SMALL_EPSILON :
    ray.dot(m_plane.m_plane_normal) > SMALL_EPSILON;
    if (false_ray_check) return false;
    if (!isWithinPolygon(p)) {
        return false;
    }
    
    ip.m_t = t;
    ip.m_normal = ip.m_orig_normal = getNormal();
    ip.m_normal.normalize();
    return true;
}

Vector3D Polygon::getNormal() const
{
    return m_plane.m_plane_orig_normal;
}

bool Polygon::isPointOnPlane(const Point3D& p)
{
    return abs(m_plane.m_plane_normal.dot(p - m_plane.m_sample_point)) < EPSILON;
}

bool Polygon::isWithinPolygon(const Point3D& p)
{
    if (!isPointOnPlane(p)) { // machine epsilon checking
        return false;
    }
    
    
    // assuming all polygons have at least 3 edges.
    std::vector<Point3D>::iterator prev = m_vertices.begin();
    std::vector<Point3D>::iterator cur = prev + 1;
    for (; cur != m_vertices.end(); ++cur, ++prev) {
        if (!isPointWithinPolygonForEdge(p, *cur, *prev)) {
            return false;
        }
    }
    // cover it up
    if (!isPointWithinPolygonForEdge(p, m_vertices.front(), m_vertices.back())) {
        return false;
    }
    return true;
}


//void Polygon::transform(const Matrix4x4& m)
//{
//    for (std::vector<Point3D>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
//        *it = m * (*it);
//    }
//    m_plane.m_plane_normal = m.invert().transpose() * m_plane.m_plane_normal;
//}


const std::vector<Point3D> & Polygon::get_vertices()
{
    return m_vertices;
}

Point2D Polygon::get2DTextureMapCoordinates(const Point3D &p) const
{
/*
    Idea:
    1. choose a random point from vertices to be the center, and find the min/max points of all vertices relative to the NDC.
    2. get the width and height of the bounding rect for these points relative to the NDC.
    3. use width, height and p, and calculate u and v.
 */
    // 1
    Point2D coords;
    double min_x, min_y, max_x, max_y;
    min_x = min_y = DBL_MAX;
    max_x = max_y = -DBL_MAX;
    Point3D sample = m_vertices[(rand() % m_vertices.size())];
    for (unsigned int i = 0; i < m_vertices.size(); ++i) {
        if (solve3x2linearSystems(m_plane.m_up_vec, m_plane.m_right_vec, m_vertices[i]-sample, coords)) {
            min_x = fmin(min_x, coords[0]);
            max_x = fmax(max_x, coords[0]);
            min_y = fmin(min_y, coords[1]);
            max_y = fmax(max_y, coords[1]);
        } else {
            cerr << "Polygon: cannot solve equations" << endl;
            return Point2D(-1, -1);
        }
    }
    // 2
    double width = max_x - min_x;
    double height = max_y - min_y;
    
    // 3
    if (solve3x2linearSystems(m_plane.m_up_vec, m_plane.m_right_vec, p-sample, coords)) {
        return Point2D((coords[0] - min_x)/width, (coords[1] - min_y)/height);
    }
    return Point2D(-1, -1);
}

Disk::Disk(const Point3D &center, double radius, const Vector3D &normal) :
    Shape2D(),
    m_plane(normal, center),
    m_center(center),
    m_radius(radius)
{
    
}

Disk::~Disk()
{
    
}

bool Disk::intersect(const Point3D &eye, const Vector3D &ray, IntersectionPoint &ip, double refl_id)
{
    double t = m_plane.calcDistRatio(eye, ray);
    #warning changed from t<0 to t < SMALL_EPSILON
    if (t < SMALL_EPSILON) return false;
    // inside of an object (refraction), assuming all materials have different refl index
    bool false_ray_check = (refl_id != DBL_MAX && ip.m_owner->get_material()->getRefractiveIndex() == refl_id) ?
    ray.dot(m_plane.m_plane_normal) < -SMALL_EPSILON :
    ray.dot(m_plane.m_plane_normal) > SMALL_EPSILON;
    if (false_ray_check) return false;
    Point3D p = eye + t * ray;
#warning maybe need EPSILON here.
    if ((p - m_center).length() <= m_radius) {
        ip.m_t = t;
        ip.m_normal = ip.m_orig_normal = getNormal();
        ip.m_normal.normalize();
        return true;
    }
    return false;
}

Vector3D Disk::getNormal() const
{
    return m_plane.m_plane_orig_normal;
}

Point3D Disk::getCenter() const
{
    return m_center;
}

Point2D Disk::get2DTextureMapCoordinates(const Point3D &p) const
{
    Point2D coords(-1, -1);
    if (solve3x2linearSystems(m_plane.m_up_vec, m_plane.m_right_vec, p - m_center, coords)) {
        return Point2D((coords[0] + 1.0) / 2.0, (coords[1] + 1.0) / 2.0);
    }
    return coords;
}




