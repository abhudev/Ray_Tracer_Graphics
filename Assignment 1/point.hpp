#ifndef __Point__
#define __Point__

#include <Eigen/Dense>

// Note - This class is defined just to differentiate between vector and point

class Point{
public:
    Eigen::Vector3d pt;

    Point();
    Point(const double a, const double b, const double c);
    Point(const Point& p);
};

#endif