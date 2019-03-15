#ifndef __RAYTRACE__
#define __RAYTRACE__

#include <omp.h>

#include "scene.hpp"

Object* shootRay(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, double& tmin, int& args);
void recursive_trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, int depth, Plane& bk_plane, Color& clr, std::vector<pvv>& ray_store);
void cast_rays(Scene& scn, int width, int height, int cols, int rows, int n, std::vector<Eigen::MatrixXd>& img, std::vector<std::vector<std::vector<pvv> > >& ray_structure);

#endif