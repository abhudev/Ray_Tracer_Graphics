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

    virtual bool internal_intersect(Ray& r, double& t, int& args) override;
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) override;
    virtual bool get_mesh(std::ofstream& fout) override;
    virtual void print() override;

private:
    Point center;                   // Center
    float radius;                   // Radius
};

#endif