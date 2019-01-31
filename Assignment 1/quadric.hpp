#ifndef __QUADRIC__
#define __QUADRIC__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Quadric : public Object{
public:
    Quadric(const Eigen::Matrix4d& m);

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;

private:
    Eigen::Matrix4d mat;
    // Ax^2 + 2Bxy + 2Cxz + 2Dx + Ey^2 + 2Fyz + 2Gy + Hz^2 + 2Iz + J = 0
    // A B C D
    // B E F G
    // C F H I
    // D G I J
};

#endif