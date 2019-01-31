#ifndef __PLANE__
#define __PLANE__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "color.hpp"

class Plane : public Object{
public:
    Plane();
    Plane(const Ray& r);
    Plane(const Point& p, const Eigen::Vector3d& v);
    Plane(const Ray& r, const Color& c);
    Plane(const Point& p, const Eigen::Vector3d& v, const Color& c);

    Color& get_color();

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;

private:
    Point pt;                       // Point on the plane
    double dist;                    // Distance from origin
    Eigen::Vector3d normal;         // Normal
    Color clr;                      // Color - Only used for Back Plane
};

inline Color& Plane::get_color(){
    return clr;
}

#endif