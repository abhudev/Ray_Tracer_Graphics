#ifndef __MESH__
#define __MESH__

#include "global.hpp"
#include "object.hpp"
#include "triangle.hpp"
#include "box.hpp"

class Mesh : public Object{
public:
    int id;

    Mesh(std::vector<Triangle>& ts);
    Mesh(std::string fname);

    void readObjFile(std::string fname, std::vector<Triangle>& ts);

    virtual bool internal_intersect(Ray& r, double& t, int& args) override;
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) override;
    virtual bool get_mesh(std::ofstream& fout) override;
    virtual void print() override;

    std::vector<Triangle> get_faces();

private:
	std::vector<Triangle> faces;
    Box boundingBox;
};

#endif