#include "ray.hpp"

Ray::Ray(const Point& p, const Eigen::Vector3d& d): ro(p), rd(d) {}