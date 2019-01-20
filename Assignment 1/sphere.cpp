#include "sphere.hpp"

Sphere::Sphere(const Point& p, const float r) : Object(), center(p), radius(r) {}

bool Sphere::intersect(const Ray& r, double& t){
    Eigen::Vector3d l = center - r.ro;
    double tca = l.transpose() * r.rd;
    if(tca < 0) return false;
    
    double d = l.norm() - tca*tca;
    if(d > radius*radius) return false;
    
    double thc = sqrt(radius*radius - d);
    t = tca - thc;                  // Doubt - Can it be tca + thc in any case ?
    return true;
}
