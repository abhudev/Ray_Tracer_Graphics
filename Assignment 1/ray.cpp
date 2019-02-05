#include "ray.hpp"

Ray::Ray() {}

Ray::Ray(const Point& p, const vec3d& d): ro(p), rd(d) {}

double Ray::get_t(Point& p){
    vec3d diff = p - ro;
    vec3d cross = diff.cross(rd);
    for(int i=0;i<3;i++) if(abs(cross[i]) > eps){
        printf("p: %s\n",p.toString().c_str());
        print();
        throw std::runtime_error("Point not on the ray");
    }
    for(int i=0;i<3;i++) if(rd[i] != 0) return diff[i]/rd[i];
    printf("p: %s\n",p.toString().c_str());
    print();
    throw std::runtime_error("Zero vector as Ray direction not possible");
}

void Ray::print(){
    printf("Ray: %s, %s\n",ro.toString().c_str(),toString(rd).c_str());
}