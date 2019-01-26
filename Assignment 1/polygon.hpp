#ifndef __POLYGON__
#define __POLYGON__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Polygon : public Object{
public:
    Polygon(std::vector<Point>& pts);       // Polygon edges are assumed between consecutive vertices

    virtual bool intersect(Ray& r, double& t) override;
    virtual Ray get_normal(Point& p) override;
    
    bool contained(const Point& p);

private:
    std::vector<Point> vertices;            // List of vertices
    double dist;                            // Distance from origin
    Eigen::Vector3d normal;                 // Normal
    std::vector<Eigen::Vector3d> edges      // Edges
};

#endif