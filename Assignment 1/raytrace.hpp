#ifndef __RAYTRACE__
#define __RAYTRACE__

#include <omp.h>

#include "scene.hpp"

Object* shootRay(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, double& tmin);
void recursive_trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, int depth, Plane& bk_plane, Color& clr);
void cast_rays(Scene& scn, int width, int height, int n, std::vector<Eigen::MatrixXd>& img);

#endif