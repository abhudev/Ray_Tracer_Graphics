#ifndef __OBJECT__
#define __OBJECT__

#include "global.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "texture.hpp"

class Object{    
public:
	std::string type;
    // :: Surface material properties ::
    double reflectivity = 0.001;
    double transparency = 0;
    double mu = 1.33;
    double s_exp = 1;
    vec3d kd = {0.2,0.2,0.2};
    vec3d ks = {0.3,0.3,0.3};
    vec3d ka = {0.3,0.6,0.2};

    // :: Transformation matrix and origin offset ::
    bool transform = false;
    vec3d d0 = {0, 0, 0};
    mat3d M = mat3d::Identity();
    mat3d Minv = mat3d::Identity();
    mat3d MT = mat3d::Identity();
    mat3d MTinv = mat3d::Identity();

    // :: Texture ::
    bool hasTexture = false;
    Texture* txt;

    Object();
    Object(std::string t);

    // Extra args for Box and Mesh Normal handling
    virtual bool internal_intersect(Ray& r, double& t, int& args) = 0;		// Writes the t to a passed variable
    virtual bool internal_get_normal(Point& p, Ray& r, int& args) = 0;
    virtual bool get_mesh(std::ofstream& fout) = 0;
    virtual void print() = 0;

    void printProperties();
    bool intersect(Ray& r, double& t, int& args);
    bool get_normal(Point& p, Ray& r, int& args);
};

#endif