#include "point.hpp"

Point::Point(): pt(0,0,0) {}

Point::Point(const double a, const double b, const double c): pt(a,b,c) {}

Point::Point(const vec3d& v): pt(v) {}

Point::Point(const Point& p): pt(p.pt) {}

double Point::distance(Point& p){
    return (pt-p.pt).norm();
}

void Point::print(){
    printf("Point: <%f, %f, %f>\n",pt[0],pt[1],pt[2]);
}

const char* Point::toString(){
    std::stringstream ss;
    ss << "<" << pt[0] << ", " << pt[1] << ", " << pt[2] << ">";
    return ss.str().c_str();
}