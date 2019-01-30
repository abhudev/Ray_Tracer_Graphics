#ifndef __RAY__
#define __RAY__

#include "global.hpp"
#include "point.hpp"

class Ray{
public:
    Point ro;                       // Ray Origin
    Eigen::Vector3d rd;             // Ray Direction

    Ray();
    Ray(const Point& p, const Eigen::Vector3d& d);
    Ray& operator= (const Ray& r);

    Point get_pt(double t);
    double get_t(Point& p);
};

inline Ray& Ray::operator= (const Ray& r){
    ro = r.ro; rd = r.rd;
    return *this;
}

inline Point Ray::get_pt(double t){
    return Point(ro.pt + rd*t);
}

#endif