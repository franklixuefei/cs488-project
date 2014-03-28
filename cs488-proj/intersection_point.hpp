//
//  intersection_point.h
//  cs488-proj
//
//  Created by Frank Li on 2014-03-24.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#ifndef __cs488_proj__intersection_point__
#define __cs488_proj__intersection_point__

#include <iostream>
#include "algebra.hpp"
using namespace std;
class GeometryNode;

struct IntersectionPoint {
	Point3D m_point;
    Point3D m_orig_point; // pre-transformation point.
	GeometryNode *m_owner;
	double m_t;
	Vector3D m_normal;
    Vector3D m_orig_normal; // pre-normalized normal;
	IntersectionPoint() :
    m_owner(NULL),
    m_t(0.0f)
	{}
	bool operator<(const IntersectionPoint& other) {
		return m_t < other.m_t;
	}
	bool operator>(const IntersectionPoint& other) {
		return m_t > other.m_t;
	}
	IntersectionPoint &operator=(const IntersectionPoint& other) {
		this->m_t = other.m_t;
		this->m_point = other.m_point;
        this->m_orig_point = other.m_orig_point;
		this->m_owner = other.m_owner;
		this->m_normal = other.m_normal;
        this->m_orig_normal = other.m_orig_normal;
		return *this;
	}
};

#endif /* defined(__cs488_proj__intersection_point__) */
