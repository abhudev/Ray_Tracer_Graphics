#ifndef __COLOR__
#define __COLOR__

#include <Eigen/Dense>

class Color{
public:
    Eigen::Vector3d rgb;

    Color();
    Color(const double a, const double b, const double c);
    Color(const Eigen::Vector3d& c);
    Color(const Color& c);

    Color operator+ (const Color& c) const;
    Color operator* (const double& d) const;
};

inline Color Color::operator+ (const Color& c) const{
    return Color(rgb + c.rgb);
}

inline Color Color::operator* (const double& d) const{
    return Color(rgb * d);
}

#endif