#include "plane.hpp"

Plane::Plane(const double d, const Ray& r): Object(), dist(d), normal(r) {}

bool Plane::intersect(const Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    // Handle the case when vd is positive - Refer slides

    t = -1*(normal.dot(r.ro.pt) + dist)/vd;
    return true;
}