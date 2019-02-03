#ifndef __SPHERE__
#define __SPHERE__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Sphere : public Object{
public:
    int id;

    Sphere(const Point& p, const float r);

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;
    virtual void print() override;

private:
    Point center;                   // Center
    float radius;                   // Radius
};

#endif