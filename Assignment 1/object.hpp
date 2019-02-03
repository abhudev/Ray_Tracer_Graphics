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
    vec3d kd = {0.2,0.2,0.2};
    vec3d ks = {0.3,0.3,0.3};
    vec3d ka = {0.3,0.6,0.2};

    Object();

    virtual bool intersect(Ray& r, double& t) = 0;          // Writes the t to a passed variable
    virtual bool get_normal(Point& p, Ray& r) = 0;
    virtual void print() = 0;

    void printProperties();
};

#endif