#ifndef __CYLINDRICAL__
#define __CYLINDRICAL__

#include "global.hpp"
#include "texture.hpp"

class CylindricalTexture : public Texture{
public:
    Point center;
    vec3d x_axis;
    vec3d y_axis;
    vec3d z_axis;

    CylindricalTexture(std::vector<Eigen::MatrixXd>& img, Point& pt, vec3d& x, vec3d& z);

    virtual vec3d getTexture(vec3d pt) override;
};

#endif