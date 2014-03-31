//
//  formulas.cpp
//  cs488-proj
//
//  Created by Frank Li on 2014-03-29.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#include "formulas.hpp"
#include <cstdlib>
using namespace std;


Vector3D Formulas::cosineWeightDiffuseReflection(const Vector3D &n)
{
    Vector3D dir;
    double rand1 = 1.0 * rand() / RAND_MAX; // [0,1]
    double rand2 = 1.0 * rand() / RAND_MAX; // [0,1]
    double theta = 2.0 * M_PI * rand1;
    double cosPhi = sqrt(rand2);
    Vector3D w = n, u = (fabs(w[0]) > 0.1 ? Vector3D(0,1,0) : Vector3D(1,0,0)).cross(w);
    u.normalize();
    Vector3D v = w.cross(u);
    dir = cos(theta)*cosPhi*u + sin(theta)*cosPhi*v + sqrt(1-rand2)*w;
    return dir; // unnomalized
}

Vector3D Formulas::pureRandomDiffuseReflection(const Vector3D &n)
{
    double x, y, z;
    Vector3D dir;
    
    /* reject any direction that is opposite to the surface normal */
    do {
        /* determine new direction by rejection sampling */
        x = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        y = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        z = (1.0*rand()/RAND_MAX - 0.5) * 2.0;
        dir = Vector3D(x, y, z);
    } while(x*x + y*y + z*z > 1.0 && dir.dot(n) > SMALL_EPSILON);
    
    return dir; // unnormalized
}

double Formulas::schlickApproxFresnelReflectance(double na, double nb, double cosTheta)
{
    // calculate reflectance using Fresnel's Formula
    double a = nb - na, b = nb + na;
    double F0 = a * a / (b * b);
    double c = 1.0 - cosTheta;
    double Fr = F0 + (1.0 - F0) * c * c * c * c * c;
    return Fr;
}

bool Formulas::SnellRefraction(
                               const Vector3D &v_dir,
                               const Vector3D &normal,
                               double na,
                               double nb,
                               Vector3D &t_dir,
                               double &nnt,
                               double &ddn)
{
    Vector3D n = normal;
    n.normalize();
    Vector3D nl= normal.dot(v_dir) < 0 ? normal : -1.0 * normal;
    nl.normalize();
    nnt = na / nb;
    bool into = normal.dot(nl)>0;
    ddn = v_dir.dot(nl);
    double cos2t = 1.0 - nnt*nnt*(1-ddn*ddn);
    if ((cos2t) < SMALL_EPSILON) { // total internal reflection);
        return false;
    }
    t_dir = nnt * v_dir - (into? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)) * n;
    return true;
}

Vector3D Formulas::perfectReflection(const Vector3D &n, const Vector3D &v_dir)
{
    return v_dir - 2 * v_dir.dot(n) * n;
}

