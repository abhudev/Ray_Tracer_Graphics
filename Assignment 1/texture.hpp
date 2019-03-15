#ifndef __TEXTURE__
#define __TEXTURE__

#include <math.h>

#include "global.hpp"
#include "point.hpp"

class Texture{
public:
    int id;
    std::vector<Eigen::MatrixXd> texture;
    int height, width;
    int mode = 0;       // Interpolation of texture space

    Texture(std::vector<Eigen::MatrixXd>& img);

    virtual vec3d getTexture(vec3d pt) = 0;
};

#endif