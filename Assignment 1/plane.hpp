#ifndef __PLANE__
#define __PLANE__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "color.hpp"

class Plane : public Object{
public:
    int id;

    Plane();
    Plane(const Ray& r);
    Plane(const Point& p, const vec3d& v);
    Plane(const Ray& r, const Color& c);
    Plane(const Point& p, const vec3d& v, const Color& c);

    Color& get_color();

    virtual bool internal_intersect(Ray& r, double& t, int& args) override;
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) override;
    virtual bool get_mesh(std::ofstream& fout) override;
    virtual void print() override;

private:
    Point pt;                       // Point on the plane
    double dist;                    // Distance from origin
    vec3d normal;                   // Normal
    Color clr;                      // Color - Only used for Back Plane
};

inline Color& Plane::get_color(){
    return clr;
}

#endif