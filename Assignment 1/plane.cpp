#include "plane.hpp"

Plane::Plane() : id(obj_count++) {}

Plane::Plane(const Ray& r): Object(), id(obj_count++), pt(r.ro), normal(r.rd) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const vec3d& v): Object(), id(obj_count++), pt(p), normal(v) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

Plane::Plane(const Ray& r, const Color& c): Object(), id(obj_count++), pt(r.ro), normal(r.rd), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const vec3d& v, const Color& c): Object(), id(obj_count++), pt(p), normal(v), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

bool Plane::intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    if(tmp < 0) return false;
    t = tmp;
    return true;
}

bool Plane::get_normal(Point& p, Ray& r){
    if(abs(normal.dot(p-pt)) > eps){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not on the plane");
        }
        return false;
    }
    r = Ray(p,normal);
    return true;
}

void Plane::print(){
    printf("ID - %d | Type - Plane\n",id);
    printf("pt: %s\n",pt.toString().c_str());
    printf("n: %s\n",toString(normal).c_str());
    printf("d: %f\n",dist);
}