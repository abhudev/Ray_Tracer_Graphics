#ifndef __PLANAR__
#define __PLANAR__

#include "global.hpp"
#include "texture.hpp"
#include "plane.hpp"

class PlanarTexture : public Texture{
public:
    vec3d x_axis;
    vec3d y_axis;
    vec3d normal;

    PlanarTexture(std::vector<Eigen::MatrixXd>& img, vec3d& z, vec3d& x);

    virtual vec3d getTexture(vec3d pt) override;
};

#endif