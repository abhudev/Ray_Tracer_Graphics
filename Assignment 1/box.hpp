#ifndef __BOX__
#define __BOX__

#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "polygon.hpp"

class Box : public Object{
public:
    // p1 (a) - p2 (b) : Length; p1 (b) - p3 (d) : Breadth; p1 (a) - p4 (e) : Height
    Box(const Point& p1, const Point& p2, const Point& p3, const Point& p4);

    virtual bool intersect(Ray& r, double& t) override;
    virtual bool get_normal(Point& p, Ray& r) override;

private:
    std::vector<Point> vertices;        // a b | e f
                                        // d c | h g
    std::vector<Polygon> faces;
};

#endif