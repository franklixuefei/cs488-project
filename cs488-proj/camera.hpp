//
//  camera.h
//  cs488-proj
//
//  Created by Frank Li on 2014-03-23.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#ifndef __cs488_proj__camera__
#define __cs488_proj__camera__

#include <iostream>
#include "algebra.hpp"
#include <cmath>
using namespace std;

#define PI_ON_180 (0.0174532925199432957)


struct CameraProperties {
    CameraProperties(
        double screen_width,
        double screen_height,
        double screen_dist,
        const Vector3D &view_dir,
        const Vector3D &left_dir,
        const Vector3D &up_dir,
        double pixel_size=1,
        size_t num_samplings=1
                     ) :
        m_screen_width(screen_width),
        m_screen_height(screen_height),
        m_screen_dist(screen_dist),
        m_view_dir(view_dir),
        m_left_dir(left_dir),
        m_up_dir(up_dir),
        m_pixel_size(pixel_size),
        m_num_samplings(num_samplings)
    {}
    double m_screen_width;
    double m_screen_height;
    double m_screen_dist;
    Vector3D m_view_dir; // -w
    Vector3D m_left_dir; // u
    Vector3D m_up_dir; // v
    double m_pixel_size; // size of virtual pixel
    size_t m_num_samplings; // # of samplings per virtual pixel
};

class Camera {
public:
    Camera(CameraProperties *properties);
    virtual ~Camera();
    virtual bool gen_ray(double dx, double dy, Vector3D &ray_dir) = 0;
protected:
    CameraProperties *m_properties;
    
};

class PinholeCamera : public Camera {
public:
    PinholeCamera(CameraProperties *properties);
    virtual ~PinholeCamera();
    bool gen_ray(double dx, double dy, Vector3D &ray_dir);
};

class FishEyeCamera : public Camera {
public:
    FishEyeCamera(
        CameraProperties *properties,
        double psi_max
    );
    virtual ~FishEyeCamera();
    bool gen_ray(double dx, double dy, Vector3D &ray_dir);
private:
    double m_psi_max;
};


#endif /* defined(__cs488_proj__camera__) */
