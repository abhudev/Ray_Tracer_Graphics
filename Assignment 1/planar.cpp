#include "planar.hpp"

PlanarTexture::PlanarTexture(std::vector<Eigen::MatrixXd>& img, vec3d& z, vec3d& x) : Texture(img), normal(z) {
    normal.normalize();
    x_axis = x - (x.dot(normal))*normal;
    x_axis.normalize();
    y_axis = normal.cross(x_axis);
}

vec3d PlanarTexture::getTexture(vec3d pt){
    double x = pt.dot(x_axis);
    double y = pt.dot(y_axis);
    
    vec3d clr(0,0,0);
    if(mode == 0){
        int idx = ((int)floor(x))%height + height;
        int idy = ((int)floor(y))%width + width;
        for(int i=0;i<3;i++){
            clr(i) = (texture[i](idx%height, idy%width) + texture[i]((idx+1)%height, idy%width) + texture[i](idx%height, (idy+1)%width) + texture[i]((idx+1)%height, (idy+1)%width))/4;
        }
    }
    return clr;
}