#include "sphere.hpp"

Sphere::Sphere(const Point& p, const float r) : Object(), center(p), radius(r) {}

bool Sphere::intersect(Ray& r, double& t){
    r.rd.normalize();
    Eigen::Vector3d l = center - r.ro;
    double tca = l.dot(r.rd);
    if(tca < 0) return false;
    
    double d = l.squaredNorm() - tca*tca;
    if(d > radius*radius) return false;
    
    double thc = sqrt(radius*radius - d);    
    t = tca < thc ? tca + thc : tca - thc;
    // printf("(%f %f) : %f\n", tca-thc, tca+thc, t);
    return true;
}

void Sphere::get_normal(Point& p, Ray& r){
    if(abs((p-center).norm() - radius) > eps){
        printf("p: %f, %f, %f\n",p.pt[0],p.pt[1],p.pt[2]);
        printf("c: %f, %f, %f | r: %f\n",center.pt[0],center.pt[1],center.pt[2],radius);
        throw std::runtime_error("Point not on sphere");
    }
    r = Ray(p,(p-center).normalized());
}