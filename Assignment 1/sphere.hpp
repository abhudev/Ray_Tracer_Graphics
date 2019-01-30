#ifndef __SPHERE__
#define __SPHERE__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Sphere : public Object{
public:
    Sphere(const Point& p, const float r);

    virtual bool intersect(Ray& r, double& t) override;
    virtual void get_normal(Point& p, Ray& r) override;

private:
    Point center;                   // Center
    float radius;                   // Radius
};

#endif