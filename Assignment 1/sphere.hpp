#ifndef __SPHERE__
#define __SPHERE__

#include "object.hpp"

class Sphere : public Object{
public:
    Sphere(const Point& p, const float r);

    virtual bool intersect(const Ray& r, double& t) override;

private:
    Point center;                   // Center
    float radius;                   // Radius
};

#endif