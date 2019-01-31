#include "camera.hpp"

Camera::Camera() : pt() {}

Camera::Camera(const Point& p) : pt(p) {}

bool Camera::intersect(Ray& r, double& t){
    throw std::runtime_error("Camera object does not implement intersect");
}

bool Camera::get_normal(Point& p, Ray& r){
    throw std::runtime_error("Camera object does not implement get_normal");
}