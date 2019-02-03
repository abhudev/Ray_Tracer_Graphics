#include "sphere.hpp"

Sphere::Sphere(const Point& p, const float r) : Object(), id(obj_count++), center(p), radius(r) {}

bool Sphere::intersect(Ray& r, double& t){
    r.rd.normalize();
    vec3d l = center - r.ro;
    double tca = l.dot(r.rd);
    if(tca < 0) return false;
    
    double d = l.squaredNorm() - tca*tca;
    if(d > radius*radius) return false;
    
    double thc = sqrt(radius*radius - d);    
    t = tca < thc ? tca + thc : tca - thc;
    return true;
}

bool Sphere::get_normal(Point& p, Ray& r){
    if(abs((p-center).norm() - radius) > eps){
        if(debug){
            printf("p: %s\n",p.toString());
            print();
            throw std::runtime_error("Point not on sphere");
        }
        return false;
    }
    r = Ray(p,(p-center).normalized());
    return true;
}

void Sphere::print(){
    printf("ID - %d | Type - Sphere\n",id);
    printf("radius: %f\n",radius);
    printf("center: %s\n",center.toString());
}