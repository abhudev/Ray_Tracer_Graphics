#ifndef __PLANE__
#define __PLANE__

#include "object.hpp"

class Plane : public Object{
public:
    Plane(const double d, const Ray& r);

    virtual bool intersect(const Ray& r, double& t) override;

private:
    double dist;                    // Distance from origin
    Eigen::Vector3d normal;         // Normal
};

#endif