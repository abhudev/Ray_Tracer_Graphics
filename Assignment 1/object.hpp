#ifndef __OBJECT__
#define __OBJECT__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"

class Object{    
public:
    // :: Surface material properties ::
    double reflectivity = 0.001;
    double transparency = 0;
    double mu = 1.33;
    double s_exp = 1;
    vec3d kd = {0.2,0.2,0.2};
    vec3d ks = {0.3,0.3,0.3};
    vec3d ka = {0.3,0.6,0.2};

    // :: Transformation matrix and origin offset ::
    bool transform = false;
    vec3d d0 = {0, 0, 0};
    mat3d M = mat3d::Identity();
    mat3d Minv = mat3d::Identity();
    mat3d MT = mat3d::Identity();
    mat3d MTinv = mat3d::Identity();

    Object();
    
    virtual bool internal_intersect(Ray& r, double& t) = 0;          // Writes the t to a passed variable
    virtual bool internal_get_normal(Point& p, Ray& r) = 0;
    virtual void print() = 0;

    void printProperties();
    bool intersect(Ray& r, double& t);
    bool get_normal(Point& p, Ray& r);
};

#endif