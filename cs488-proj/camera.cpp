//
//  camera.cpp
//  cs488-proj
//
//  Created by Frank Li on 2014-03-23.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//


#include "camera.hpp"


Camera::Camera(CameraProperties *properties) :
    m_properties(properties)
{}

Camera::~Camera()
{
    delete m_properties;
}

PinholeCamera::PinholeCamera(CameraProperties *properties) :
    Camera(properties)
{}

PinholeCamera::~PinholeCamera()
{}

bool PinholeCamera::gen_ray(double dx, double dy, Vector3D &ray_dir)
{
	ray_dir =   m_properties->m_screen_dist * m_properties->m_view_dir +
                dx * m_properties->m_left_dir +
                dy * m_properties->m_up_dir;
	return true;
}

FishEyeCamera::FishEyeCamera(
                             CameraProperties *properties,
                             double psi_max
                             ) :
    Camera(properties),
    m_psi_max(psi_max)
{}

FishEyeCamera::~FishEyeCamera()
{}


bool FishEyeCamera::gen_ray(double dx, double dy, Vector3D &ray_dir)
{
    Vector3D u = 1.0 * m_properties->m_left_dir;
    Vector3D v = 1.0 * m_properties->m_up_dir;
    Vector3D w = -1.0 * m_properties->m_view_dir;
    u.normalize();
    v.normalize();
    w.normalize();
    Point2D pn(m_properties->m_pixel_size * 2.0 / (m_properties->m_pixel_size * m_properties->m_screen_width) * dx,
               m_properties->m_pixel_size * 2.0 / (m_properties->m_pixel_size * m_properties->m_screen_height) * dy);
	double r_squared = pn[0] * pn[0] + pn[1] * pn[1];
    
	if (r_squared <= 1.0) {
		float r 		= sqrt(r_squared);
		float psi 		= r * m_psi_max * PI_ON_180;
		float sin_psi 	= sin(psi);
		float cos_psi 	= cos(psi);
		float sin_alpha = pn[1] / r;
		float cos_alpha = pn[0] / r;
		ray_dir = sin_psi * cos_alpha * u +  sin_psi * sin_alpha * v - cos_psi * w;
        
		return true;
	}
	return false;
}

