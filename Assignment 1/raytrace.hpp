#ifndef __RAYTRACE__
#define __RAYTRACE__

#include "global.hpp"
#include "object.hpp"
#include "plane.hpp"
#include "light.hpp"
#include "color.hpp"
#include "camera.hpp"

Object* shootRay(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, double& tmin);
void trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, Plane& bk_plane, Color& clr);
void recursive_trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, int depth, Plane& bk_plane, Color& clr);
void cast_rays(Point& cam_vcs, Point& cam_wcs, Eigen::Matrix3d& vcs_mat, Object* cam, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, Plane& bk_plane, int X, int Y, int n, Eigen::MatrixXd& img1, Eigen::MatrixXd& img2, Eigen::MatrixXd& img3);

#endif