#ifndef __OBJECT__
#define __OBJECT__

#include "ray.hpp"

class Object{
public:
    Object();

    virtual bool intersect(const Ray& r, double& t) = 0;

protected:              // Or should it be public
    // material;
};

#endif