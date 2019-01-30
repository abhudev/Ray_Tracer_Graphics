#include "box.hpp"

Box::Box(const Point& p1, const Point& p2, const Point& p3, const Point& p4): Object() {
    vertices.push_back(p1);                                                 // a
    vertices.push_back(p2);                                                 // b
    vertices.push_back(Point(p1.pt+p3.pt-p2.pt));                           // c
    vertices.push_back(p3);                                                 // d
    vertices.push_back(p4);                                                 // e
    vertices.push_back(Point(p2.pt+p4.pt-p1.pt));                           // f
    vertices.push_back(Point(vertices[2].pt+vertices[5].pt-p1.pt));         // g
    vertices.push_back(Point(vertices[6].pt+p4.pt-vertices[5].pt));         // h

    std::vector<Point> f1{vertices[0],vertices[1],vertices[2],vertices[3]};     // abcd
    std::vector<Point> f2{vertices[7],vertices[6],vertices[5],vertices[4]};     // efgh
    std::vector<Point> f3{vertices[0],vertices[4],vertices[5],vertices[1]};     // aefb
    std::vector<Point> f4{vertices[7],vertices[3],vertices[2],vertices[6]};     // gcdh
    std::vector<Point> f5{vertices[4],vertices[0],vertices[3],vertices[7]};     // eadh
    std::vector<Point> f6{vertices[1],vertices[2],vertices[6],vertices[5]};     // fgcb

    faces.push_back(Polygon(f1));
    faces.push_back(Polygon(f2));
    faces.push_back(Polygon(f3));
    faces.push_back(Polygon(f4));
    faces.push_back(Polygon(f5));
    faces.push_back(Polygon(f6));
}

bool Box::intersect(Ray& R, double& t){
    double tmp = t;
    std::vector<double> ts(6,t);
    for(int i=0;i<6;i++) faces[i].intersect(R,ts[i]);
    bool hit = false;
    for(int i=0;i<6;i++) if(ts[i] != tmp){
        t = ts[i] < t ? ts[i] : t;
        hit = true;
    }
    return hit;
}

void Box::get_normal(Point& p, Ray& r){
    std::vector<bool> on(6,false);
    for(int i=0;i<6;i++) if(faces[i].contained(p)) on[i] = true;
    int s = 0;
    for(int i=0;i<6;i++) s += on[i];
    if(s < 1) throw std::runtime_error("Point inside/outside the Box");
    if(s > 1) throw std::runtime_error("Point on the edge/corner");
    Ray n;
    for(int i=0;i<6;i++) if(on[i]){
        r = Ray(p,faces[i].get_this_normal());
    }
}