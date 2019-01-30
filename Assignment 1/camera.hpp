#ifndef __CAMERA__
#define __CAMERA__

#include "object.hpp"

class Camera : public Object{
public:
    Point pt;

    Camera();
    Camera(const Point& p);

    virtual bool intersect(Ray& r, double& t) override;
    virtual void get_normal(Point& p, Ray& r) override;
};

#endif