#include "quadric.hpp"
#include <math.h>

Quadric::Quadric(const Eigen::Matrix4d& m): Object(), mat(m) {}

bool Quadric::intersect(Ray& r, double& t){
    Eigen::Vector4d d(r.rd[0],r.rd[1],r.rd[2],1);
    Eigen::Vector4d p(r.ro.pt[0],r.ro.pt[1],r.ro.pt[2],1);
    double a = d.transpose() * mat * d;
    double b = 2 * d.transpose() * mat * p;
    double c = p.transpose() * mat * p;

    if(a == 0){
        t = -1*c/b;
        return true;
    }
    double disc = b*b - 4*a*c;
    if(disc < 0) return false;
    double disc_sq = sqrt(disc);
    t = -1*b < disc_sq ? (disc_sq-b)/(2*a) : (-1*b-disc_sq)/(2*a);
    return true;
}