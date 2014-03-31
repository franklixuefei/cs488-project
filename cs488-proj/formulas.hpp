//
//  formulas.h
//  cs488-proj
//
//  Created by Frank Li on 2014-03-29.
//  Copyright (c) 2014 Xuefei Li. All rights reserved.
//

#ifndef __cs488_proj__formulas__
#define __cs488_proj__formulas__

#include <iostream>
#include "algebra.hpp"
using namespace std;

class Formulas {
public:
    static Vector3D cosineWeightDiffuseReflection(const Vector3D &n);
    static Vector3D pureRandomDiffuseReflection(const Vector3D &n);
    static double schlickApproxFresnelReflectance(double na, double nb, double cosTheta);
    static bool SnellRefraction(
                                const Vector3D &v_dir,
                                const Vector3D &normal,
                                double na,
                                double nb,
                                Vector3D &t_dir,
                                double &nnt,
                                double &ddn);
    static Vector3D perfectReflection(const Vector3D &n, const Vector3D &v_dir);
};

#endif /* defined(__cs488_proj__formulas__) */
