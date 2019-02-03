#include "camera.hpp"

Camera::Camera() : pt(), look_at() {}

Camera::Camera(const Point& p, const vec3d l) : pt(p), look_at(l) {}

bool Camera::intersect(Ray& r, double& t){
    throw std::runtime_error("Camera object does not implement intersect");
}

bool Camera::get_normal(Point& p, Ray& r){
    throw std::runtime_error("Camera object does not implement get_normal");
}

void Camera::print(){
    printf("Type - Camera\n");
    printf("loc: %s\n",pt.toString());
    printf("look_at: %s\n",toString(look_at));
}