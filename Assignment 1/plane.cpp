#include "plane.hpp"

Plane::Plane() : Object("Plane"), id(obj_count++) {}

Plane::Plane(const Ray& r): Object("Plane"), id(obj_count++), pt(r.ro), normal(r.rd) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const vec3d& v): Object("Plane"), id(obj_count++), pt(p), normal(v) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

Plane::Plane(const Ray& r, const Color& c): Object("Plane"), id(obj_count++), pt(r.ro), normal(r.rd), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(r.ro.pt);
}

Plane::Plane(const Point& p, const vec3d& v, const Color& c): Object("Plane"), id(obj_count++), pt(p), normal(v), clr(c) {
    normal.normalize();
    dist = -1*normal.dot(p.pt);
}

bool Plane::internal_intersect(Ray& r, double& t, int& args){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    if(tmp < 0) return false;
    t = tmp;
    return true;
}

bool Plane::internal_get_normal(Point& p, Ray& r, int& args){
    if(abs(normal.dot(p-pt)) > eps){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not on the plane");
        }
        return false;
    }
    r = Ray(p,normal);
    return true;
}

void Plane::print(){
    printf("ID - %d | Type - Plane\n",id);
    printf("pt: %s\n",pt.toString().c_str());
    printf("n: %s\n",toString(normal).c_str());
    printf("d: %f\n",dist);
}

bool Plane::get_mesh(std::ofstream& fout)
{
    vec3d d1, d2;    
    d1[0] = 1, d1[1] = 1; d1[2] = 1;
    d2[0] = -1, d2[1] = 1; d2[2] = 2;

    if(abs(normal[2]) > eps) {
        d1[2] = -1.0*(normal[0]*d1[0]+normal[1]*d1[1])/normal[2];
        d2[2] = -1.0*(normal[0]*d2[0]+normal[1]*d2[1])/normal[2];
    }
    else if(abs(normal[1]) > eps) {
        d1[1] = -1.0*(normal[0]*d1[0]+normal[2]*d1[2])/normal[1];
        d2[1] = -1.0*(normal[0]*d2[0]+normal[2]*d2[2])/normal[1];
    }
    else {
        d1[0] = -1.0*(normal[1]*d1[1]+normal[2]*d1[2])/normal[0];
        d2[0] = -1.0*(normal[1]*d2[1]+normal[2]*d2[2])/normal[0];
    }

    vec3d p1, p2, p3, p4;
    p1 = pt.pt - 100*d1;
    p2 = pt.pt - 100*d2;
    p3 = pt.pt + 100*d1;
    p4 = pt.pt + 100*d2;

    fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
    fout<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<"\n";
    fout<<"v "<<p4[0]<<" "<<p4[1]<<" "<<p4[2]<<"\n";
    fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
    fout<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<"\n";
    fout<<"v "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<"\n";

    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
    fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";    

    return true;
}