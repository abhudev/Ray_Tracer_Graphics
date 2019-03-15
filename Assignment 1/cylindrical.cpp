#include "cylindrical.hpp"

CylindricalTexture::CylindricalTexture(std::vector<Eigen::MatrixXd>& img, Point& pt, vec3d& x, vec3d& z) : Texture(img), center(pt), z_axis(z) {
    z_axis.normalize();
    x_axis = x - (x.dot(z_axis))*z_axis;
    x_axis.normalize();
    y_axis = z_axis.cross(x_axis);
}

vec3d CylindricalTexture::getTexture(vec3d pt){
    vec3d rd = pt - center.pt;
    double theta = atan2(rd.dot(y_axis),rd.dot(x_axis));
    double x = theta/(2*PI) + 0.5;
    double y = rd.dot(z_axis);

    vec3d clr(0,0,0);
    if(mode == 0){
        int idx = floor(x*height), idy = ((int)floor(y))%width + width;
        for(int i=0;i<3;i++){
            clr(i) = (texture[i](idx%height, idy%width) + texture[i]((idx+1)%height, idy%width) + texture[i](idx%height, (idy+1)%width) + texture[i]((idx+1)%height, (idy+1)%width))/4;
        }
    }
    return clr;
}