#ifndef __CAMERA__
#define __CAMERA__

#include "object.hpp"

class Camera : public Object{
public:
    Point pt;
    vec3d look_at;

    Camera();
    Camera(const Point& p, const vec3d l);

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;
    virtual void print() override;
};

#endif