#include "ray.hpp"

Ray::Ray() {}

Ray::Ray(const Point& p, const Eigen::Vector3d& d): ro(p), rd(d) {}

double Ray::get_t(Point& p){
    std::vector<bool> chk(3,true);
    std::vector<double> tv(3,0);
    Eigen::Vector3d diff = p - ro;
    Eigen::Vector3d cross = diff.cross(rd);
    for(int i=0;i<3;i++) if(abs(cross[i]) > eps) throw std::runtime_error("Point not on the ray");
    for(int i=0;i<3;i++){
        if(rd[i] != 0) return diff[i]/rd[i];
    }
    throw std::runtime_error("Zero vector as Ray direction not possible");
}