#ifndef __SCENE__
#define __SCENE__

#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "global.hpp"

#include "object.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "quadric.hpp"
#include "polygon.hpp"
#include "box.hpp"
#include "triangle.hpp"
#include "mesh.hpp"

#include "texture.hpp"
#include "planar.hpp"
#include "spherical.hpp"
#include "cylindrical.hpp"

#include "light.hpp"
#include "camera.hpp"
#include "color.hpp"

class Scene{
public:
    std::vector<Object*> objs;
    std::vector<LightSrc> lt_srcs;
    Plane bk_plane;
    Camera cam;
    mat3d vcs_mat;
    Point vcs_origin;
    Point cam_wcs;

    Scene();
    Scene(std::vector<Object*>& o, std::vector<LightSrc>& l, Plane& bk, Camera& c, mat3d& mat, Point& origin);
    Scene(std::string filename);

    void generateScene(std::string filename);
private:
    void defineCamera(json& j_cam);
    void defineVCS(json& j_vcs);
    void defineBackPlane(json& j_bk);
    void placeLightSources(json& j_lts);
    void placeObjects(json& j_obj);
};

#endif