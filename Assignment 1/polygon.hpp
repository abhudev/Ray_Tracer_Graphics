#ifndef __POLYGON__
#define __POLYGON__

#include "object.hpp"

class Polygon : public Object{
public:
    Polygon(std::vector<Point>& pts);       // Polygon edges are assumed between consecutive vertices

    virtual bool intersect(const Ray& r, double& t) override;

private:
    std::vector<Point> vertices;            // List of vertices
    Eigen::Vector3d normal;                 // Normal
};

#endif