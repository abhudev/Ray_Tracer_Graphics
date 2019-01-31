#include "quadric.hpp"
#include <math.h>

Quadric::Quadric(const Eigen::Matrix4d& m): Object(), mat(m) {}

bool Quadric::intersect(Ray& r, double& t){
    Eigen::Vector4d d(r.rd[0],r.rd[1],r.rd[2],0);
    Eigen::Vector4d p(r.ro.pt[0],r.ro.pt[1],r.ro.pt[2],1);
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
    // Epsilon
    if(disc < 0){
        if(tmp < 0){
        // printf("disc = %f\n",disc);
        // printf("disc < 0\n");
        return false;
    }
    }
    double disc_sq = sqrt(disc);
    // tmp = -1*b < disc_sq ? (disc_sq-b)/(2*a) : (-1*b-disc_sq)/(2*a);
    t1 = (disc_sq-b)/(2*a);
    t2 = (-1*b-disc_sq)/(2*a);
    tmp = t1 > 0 && t2 > 0 ? std::min(t1,t2) : std::max(t1,t2);
    // printf("Ray dir = %f %f %f\n", d[0], d[1], d[2]);
    // printf("Ray pt = %f %f %f\n", p[0], p[1], p[2]);
    // printf("a = %f | b = %f | c = %f\n",a,b,c);
    // printf("t1 = %f | t2 = %f\n",t1, t2);
    Point p1 = r.get_pt(t1);
    Point p2 = r.get_pt(t2);
    // printf("P1 = %f %f %f\n", p1.pt[0], p1.pt[1], p1.pt[2]);
    // printf("P2 = %f %f %f\n", p2.pt[0], p2.pt[1], p2.pt[2]);
    if(tmp < 0){
        // printf("t < 0\n");
        // printf("Ray dir = %f %f %f\n", d[0], d[1], d[2]);
        // printf("Ray pt = %f %f %f\n", p[0], p[1], p[2]);
        // printf("a = %f | b = %f | c = %f\n",a,b,c);
        // printf("t1 = %f | t2 = %f\n",t1, t2);
        // Point p1 = r.get_pt(t1);
        // Point p2 = r.get_pt(t2);
        // printf("P1 = %f %f %f\n", p1.pt[0], p1.pt[1], p1.pt[2]);
        // printf("P2 = %f %f %f\n", p2.pt[0], p2.pt[1], p2.pt[2]);
        // printf("t < 0\n");
        return false;
    }
    t = tmp;
    return true;
}

// TODO - how chec point is on quadric?
bool Quadric::get_normal(Point& p, Ray& r){
    // TODO - Check if point is on the quadric
    Eigen::Vector4d p_homo(p.pt[0],p.pt[1],p.pt[2],1);
    Eigen::Vector3d n_dir;

    Eigen::Vector4d prod = mat * p_homo;
    if(abs(p_homo.dot(prod)) > eps){
        // std::cout << mat << "\n";
        // printf("Point: %f %f %f\n",p.pt[0],p.pt[1],p.pt[2]);
        // printf("Point not on Quadric\n");
        // throw std::runtime_error("Stop");
        return false;
    }

    for(int i=0;i<3;i++) for(int j=0;j<4;j++){
        n_dir[i] += mat(i,j)*p_homo[j];
    }
    n_dir.normalize();
    r = Ray(p, n_dir);
    return true;
}