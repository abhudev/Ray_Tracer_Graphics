#include "plane.hpp"

Plane::Plane() {}

Plane::Plane(const Ray& r): Object(), normal(r.rd), pt(r.ro) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const Eigen::Vector3d& v): Object(), normal(v), pt(p) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

Plane::Plane(const Ray& r, const Color& c): Object(), normal(r.rd), pt(r.ro), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const Eigen::Vector3d& v, const Color& c): Object(), normal(v), pt(p), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

bool Plane::intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    // Handle the case when vd is positive - Refer slides

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    if(tmp < 0) return false;
    t = tmp;
    return true;
}

bool Plane::get_normal(Point& p, Ray& r){
    if(abs(normal.dot(p-pt)) > eps){
        // printf("p: %f, %f, %f\n",p.pt[0],p.pt[1],p.pt[2]);
        // printf("pt: %f, %f, %f\n",pt.pt[0],pt.pt[1],pt.pt[2]);
        // printf("n: %f, %f, %f\n",normal[0],normal[1],normal[2]);
        return false;
        // throw std::runtime_error("Point not on the plane");
    }
    r = Ray(p,normal);
    return true;
}