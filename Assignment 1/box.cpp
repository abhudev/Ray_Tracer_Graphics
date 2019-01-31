#include "box.hpp"

Box::Box(const Point& p1, const Point& p2, const Point& p3, const Point& p4): Object() {
    vertices.push_back(p1);                                                 // a 0
    vertices.push_back(p2);                                                 // b 1
    vertices.push_back(Point(p2.pt+p3.pt-p1.pt));                           // c 2
    vertices.push_back(p3);                                                 // d 3
    vertices.push_back(p4);                                                 // e 4
    vertices.push_back(Point(p2.pt+p4.pt-p1.pt));                           // f 5
    vertices.push_back(Point(vertices[2].pt+vertices[5].pt-p2.pt));         // g 6
    vertices.push_back(Point(vertices[6].pt+p4.pt-vertices[5].pt));         // h 7

    std::vector<Point> f1{vertices[0],vertices[1],vertices[2],vertices[3]};     // abcd
    std::vector<Point> f2{vertices[7],vertices[6],vertices[5],vertices[4]};     // efgh
    std::vector<Point> f3{vertices[0],vertices[4],vertices[5],vertices[1]};     // aefb
    std::vector<Point> f4{vertices[7],vertices[3],vertices[2],vertices[6]};     // gcdh
    std::vector<Point> f5{vertices[4],vertices[0],vertices[3],vertices[7]};     // eadh
    std::vector<Point> f6{vertices[5],vertices[6],vertices[2],vertices[1]};     // fgcb

    faces.push_back(Polygon(f1));
    faces.push_back(Polygon(f2));
    faces.push_back(Polygon(f3));
    faces.push_back(Polygon(f4));
    faces.push_back(Polygon(f5));
    faces.push_back(Polygon(f6));
}

bool Box::intersect(Ray& R, double& t){
    t = std::numeric_limits<double>::infinity();
    double tmp = t;
    std::vector<double> ts(6);
    for(int i=0;i<6;i++)
    {      
      int chk_face = faces[i].intersect(R,ts[i]);
      if(!chk_face)
        ts[i] = std::numeric_limits<double>::infinity();
      // printf("ts[%d] %f\n", i, ts[i]);
    } 
    bool hit = false;
    for(int i=0;i<6;i++) if(ts[i] < tmp){
        t = ts[i] < t ? ts[i] : t;
        hit = true;
    }
    return hit;
}

bool Box::get_normal(Point& p, Ray& r){
    // printf("Start\n");
    std::vector<bool> on(6,false);
    for(int i=0;i<6;i++){
        if(faces[i].contained(p)){
            on[i] = true;
            // printf("On face %d\n",i);
        }
    }
    int s = 0;
    for(int i=0;i<6;i++) s += on[i];
    // printf("s = %d\n",s);
    if(s < 1) 
    {
        return false;
        // throw std::runtime_error("Point inside/outside the Box");
    }
    if(s > 1)
    {
        return false;
        // throw std::runtime_error("Point on the edge/corner");
    }
    Ray n;
    for(int i=0;i<6;i++) if(on[i]){
        r = Ray(p,faces[i].get_this_normal());
    }

    return true;
}