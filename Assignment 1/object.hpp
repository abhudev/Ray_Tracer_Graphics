#ifndef __OBJECT__
#define __OBJECT__

#include "point.hpp"
#include "ray.hpp"

class Object{
public:
    Object();

    virtual bool intersect(Ray& r, double& t) = 0;          // Writes the t to a passed variable
    virtual Ray get_normal(Point& p) = 0;

protected:              // Or should it be public
    // material;
};

#endif