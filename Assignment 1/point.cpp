#include "point.hpp"

Point::Point(): pt(0,0,0) {}

Point::Point(const double a, const double b, const double c): pt(a,b,c) {}

Point::Point(const Eigen::Vector3d& v): pt(v) {}

Point::Point(const Point& p): pt(p.pt) {}