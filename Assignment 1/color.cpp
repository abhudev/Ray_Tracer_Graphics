#include "color.hpp"

Color::Color() : rgb(0,0,0) {}

Color::Color(const double a, const double b, const double c) : rgb(a,b,c) {}

Color::Color(const vec3d& c) : rgb(c) {}

Color::Color(const Color& c) : rgb(c.rgb) {}