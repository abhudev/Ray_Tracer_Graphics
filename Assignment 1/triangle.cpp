#include "triangle.hpp"

Triangle::Triangle(Point& a, Point& b, Point& c) : Object("Triangle"), id(obj_count++){
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    normal = (b - a).cross(c - b);
    normal.normalize();
    dist = -1.0*vertices[0].pt.dot(normal);
}

bool Triangle::internal_intersect(Ray& r, double& t, int& args){
    double vd = normal.dot(r.rd);
    if(abs(vd) < eps) return false;

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    if(tmp < 0) return false;
    Point xpt = r.get_pt(tmp);
    if(!contained(xpt)) return false;
    t = tmp;
    return true;
}

bool Triangle::internal_get_normal(Point& p, Ray& r, int& args){
    if(!contained(p)){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not inside the triangle");
        }
        return false;
    }
    r = Ray(p,normal);
    return true;
}

void Triangle::print(){
    printf("ID - %d | Type - Triangle\n",id);
    printf("n: %s\n",toString(normal).c_str());
    printf("d: %f\n",dist);
    printf("Points - \n");
    for(uint i=0;i<vertices.size();++i) printf("%s\n",vertices[i].toString().c_str());
}

bool Triangle::contained(Point& p){
    if(abs(normal.dot(vertices[0] - p)) > eps) {
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not on the plane of triange");
        }
        return false;
    }

    double a = ((vertices[1] - vertices[0]).cross(vertices[2] - vertices[0])).norm()/2;
    double a0 = ((vertices[1] - p).cross(vertices[2] - p)).norm()/2;
    double a1 = ((vertices[2] - p).cross(vertices[0] - p)).norm()/2;
    double a2 = ((vertices[0] - p).cross(vertices[1] - p)).norm()/2;

    if(abs(a - (a0 + a1 + a2)) > eps) {
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not inside the triangle");
        }
        return false;
    }
    return true;
}

vec3d Triangle::get_normal(){
    return normal;
}

double Triangle::get_dist(){
    return dist;
}

bool Triangle::get_mesh(std::ofstream& fout){
    return true;
}