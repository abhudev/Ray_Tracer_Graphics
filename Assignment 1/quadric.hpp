#ifndef __QUADRIC__
#define __QUADRIC__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Quadric : public Object{
public:
    int id;

    Quadric(const mat4d& m);

    virtual bool internal_intersect(Ray& r, double& t) override;
    virtual bool internal_get_normal(Point& p, Ray& r) override;
    virtual void print() override;

private:
    mat4d mat;
    // Ax^2 + 2Bxy + 2Cxz + 2Dx + Ey^2 + 2Fyz + 2Gy + Hz^2 + 2Iz + J = 0
    // A B C D
    // B E F G
    // C F H I
    // D G I J
};

#endif