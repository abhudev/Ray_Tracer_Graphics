#ifndef __POLYGON__
#define __POLYGON__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"

class Polygon : public Object{
public:
    int id;
    Polygon(std::vector<Point>& pts);       // Polygon edges are assumed between consecutive vertices

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;
    virtual void print() override;
    
    bool contained(Point& p);
    vec3d get_this_normal();

private:
    std::vector<Point> vertices;            // List of vertices
    double dist;                            // Distance from origin
    vec3d normal;                           // Normal
    std::vector<vec3d> edges;               // Edges
};

#endif