#ifndef __RAY__
#define __RAY__

#include "point.hpp"

class Ray{
public:
    Point ro;                       // Ray Origin
    Eigen::Vector3d rd;             // Ray Direction

    Ray(const Point& p, const Eigen::Vector3d& d);
    Ray& operator= (const Ray& r);
};

inline Ray& Ray::operator= (const Ray& r){
    ro = r.ro; rd = r.rd;
    return *this;
}

#endif