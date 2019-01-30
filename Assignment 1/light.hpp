#ifndef __LIGHT__
#define __LIGHT__

#include "global.hpp"
#include "object.hpp"
#include "plane.hpp"
#include "color.hpp"
#include "camera.hpp"

class LightSrc{
public:
    Color il;
    Point loc;

    LightSrc(const Color& c, const Point& p);
};

inline double get_diffuse(const Eigen::Vector3d& normal_rd, const Eigen::Vector3d& light_rd){
    double cos = normal_rd.dot(light_rd);
    // if(cos < 0) throw std::runtime_error("cos angle negative at get_diffuse");
    if(cos <= 0) return 0;
    return cos;
}

inline double get_specular(const Eigen::Vector3d& reflected_rd, const Eigen::Vector3d& cast_rd, const double& specular_exp){
    double cos = reflected_rd.dot(cast_rd);
    // if(cos < 0) throw std::runtime_error("cos angle negative at get_specular");
    if(cos <= 0) return 0;
    return pow(cos, specular_exp);
}

inline void get_reflected(const Eigen::Vector3d& normal_rd, const Eigen::Vector3d& light_rd, Eigen::Vector3d& reflected_rd){
    // Check for Normalization
    reflected_rd = 2*(normal_rd.dot(light_rd))*normal_rd - light_rd;
}

// Returns false if Total Internal Reflection
bool get_refracted(const Eigen::Vector3d& N, const Eigen::Vector3d& I, const double& mu, Eigen::Vector3d& T);

void light_src_interaction(Point& startPoint, Object* objcur, std::vector<Object*>& list_obj, std::vector<LightSrc>& light_sources, Eigen::Vector3d& cast_rd, Plane& bk_plane, Color& clr);

#endif