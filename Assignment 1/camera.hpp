#ifndef __CAMERA__
#define __CAMERA__

#include "object.hpp"

class Camera : public Object{
public:
    Point pt;
    vec3d look_at;

    Camera();
    Camera(const Point& p, const vec3d l);

    virtual bool internal_intersect(Ray& r, double& t, int& args) override;
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) override;
    virtual bool get_mesh(std::ofstream& fout) override;
    virtual void print() override;
};

#endif