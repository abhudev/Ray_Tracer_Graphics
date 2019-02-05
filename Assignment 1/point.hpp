#ifndef __Point__
#define __Point__

#include "global.hpp"

// Note - This class is defined just to differentiate between vector and point

class Point{
public:
    vec3d pt;

    Point();
    Point(const double a, const double b, const double c);
    Point(const vec3d& v);
    Point(const Point& p);

    vec3d operator- (const Point& p) const;
    double distance(Point& p);
    void print();
    std::string toString();
};

inline vec3d Point::operator- (const Point& p) const{
    return vec3d(pt - p.pt);
}

#endif