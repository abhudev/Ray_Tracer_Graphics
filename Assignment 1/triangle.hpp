#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "global.hpp"
#include "object.hpp"

class Mesh;

class Triangle : public Object{
public:
    int id;

    Triangle(Point& a, Point& b, Point& c);

    virtual bool internal_intersect(Ray& r, double& t, int& args) override;
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) override;
    virtual bool get_mesh(std::ofstream& fout) override;
    virtual void print() override;

    bool contained(Point& p);
    double get_dist();
    vec3d get_normal();

    friend class Mesh;

private:
    std::vector<Point> vertices;
    vec3d normal;
    double dist;
};

#endif