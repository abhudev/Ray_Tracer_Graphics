#ifndef __OBJECT__
#define __OBJECT__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"

class Object{
public:
    double reflectivity = 0.001;
    double transparency = 0;
    double mu = 1.33;
    double s_exp = 1;
    Eigen::Vector3d kd = {0.2,0.2,0.2};
    Eigen::Vector3d ks = {0.3,0.3,0.3};
    Eigen::Vector3d ka = {0.3,0.6,0.2};

    Object();

    virtual bool intersect(Ray& r, double& t) = 0;          // Writes the t to a passed variable
    virtual void get_normal(Point& p, Ray& r) = 0;
};

#endif