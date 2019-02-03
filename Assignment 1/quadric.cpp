#include "quadric.hpp"
#include <math.h>

Quadric::Quadric(const mat4d& m): Object(), id(obj_count++), mat(m) {}

bool Quadric::intersect(Ray& r, double& t){
    vec4d p(r.ro.pt[0],r.ro.pt[1],r.ro.pt[2],1);
    vec4d d(r.rd[0],r.rd[1],r.rd[2],0);             // Important - Direction has 0 as homogenous coordinate
    double a = d.dot(mat * d);
    double b = 2 * d.dot(mat * p);
    double c = p.dot(mat * p);

    double tmp, t1, t2;

    if(a == 0){
        tmp = -1.0*c/b;
        if(tmp < 0) return false;
        t = tmp;
        return true;
    }
    double disc = b*b - 4*a*c;
    if(disc < 0) return false;

    double disc_sq = sqrt(disc);
    t1 = (disc_sq-b)/(2*a);
    t2 = (-1*b-disc_sq)/(2*a);
    tmp = t1 > 0 && t2 > 0 ? std::min(t1,t2) : std::max(t1,t2);
    if(tmp < 0) return false;
    t = tmp;
    return true;
}

bool Quadric::get_normal(Point& p, Ray& r){
    vec4d p_homo(p.pt[0],p.pt[1],p.pt[2],1);
    vec3d n_dir(0,0,0);

    vec4d prod = mat * p_homo;
    if(abs(p_homo.dot(prod)) > eps){
        if(debug){
            printf("p: %s\n",p.toString());
            print();
            throw std::runtime_error("Point not on the quadric");
        }
        return false;
    }

    for(int i=0;i<3;i++) for(int j=0;j<4;j++) n_dir[i] += mat(i,j)*p_homo[j];
    n_dir.normalize();
    r = Ray(p, n_dir);
    return true;
}

void Quadric::print(){
    printf("ID - %d | Type - Quadric\n",id);
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j) printf("%f ",mat(i,j));
        printf("\n");
    }
}