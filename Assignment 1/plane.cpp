#include "plane.hpp"

Plane::Plane(const Ray& r): Object(), normal(r.rd) {
    normal.normalize();
    dist = normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const Eigen::Vector3d& v): Object(), normal(v) {
    normal.normalize();
    dist = normal.dot(p.pt);
}

bool Plane::intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    // Handle the case when vd is positive - Refer slides

    t = abs(-1*(normal.dot(r.ro.pt) + dist)/vd);
    return true;
}