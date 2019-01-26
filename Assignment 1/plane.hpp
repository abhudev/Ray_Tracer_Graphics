#ifndef __PLANE__
#define __PLANE__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Plane : public Object{
public:
    Plane(const Ray& r);
    Plane(const Point& p, const Eigen::Vector3d& v);

    virtual bool intersect(Ray& r, double& t) override;
    virtual Ray get_normal(Point& p) override;

private:
    double dist;                    // Distance from origin
    Eigen::Vector3d normal;         // Normal
};

#endif