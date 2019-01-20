#ifndef __QUADRIC__
#define __QUADRIC__

#include "object.hpp"

class Quadric : public Object{
public:
    Quadric(const Eigen::Matrix4d& m);

    virtual bool intersect(const Ray& r, double& t) override;

private:
    Eigen::Matrix4d mat;
    // Ax^2 + 2Bxy + 2Cxz + 2Dx + Ey^2 + 2Fyz + 2Gy + Hz^2 + 2Iz + J = 0
    // A B C D
    // B E F G
    // C F H I
    // D G I J
};

#endif