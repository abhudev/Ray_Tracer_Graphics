#include "camera.hpp"

Camera::Camera() : pt(), look_at() {}

Camera::Camera(const Point& p, const vec3d l) : pt(p), look_at(l) {}

bool Camera::internal_intersect(Ray& r, double& t){
    throw std::runtime_error("Camera object does not implement internal_intersect");
}

bool Camera::internal_get_normal(Point& p, Ray& r){
    throw std::runtime_error("Camera object does not implement internal_get_normal");
}

void Camera::print(){
    printf("Type - Camera\n");
    printf("loc: %s\n",pt.toString().c_str());
    printf("look_at: %s\n",toString(look_at).c_str());
}