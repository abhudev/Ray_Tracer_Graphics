#include "box.hpp"

Box::Box(const Point& p1, const Point& p2, const Point& p3, const Point& p4): Object(), id(obj_count++) {
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

    faces.push_back(Polygon(f1));   faces[0].id = id;
    faces.push_back(Polygon(f2));   faces[1].id = id;
    faces.push_back(Polygon(f3));   faces[2].id = id;
    faces.push_back(Polygon(f4));   faces[3].id = id;
    faces.push_back(Polygon(f5));   faces[4].id = id;
    faces.push_back(Polygon(f6));   faces[5].id = id;
    obj_count -= 6;
}

bool Box::internal_intersect(Ray& R, double& t){
    t = std::numeric_limits<double>::infinity();
    double tmp = t;
    std::vector<double> ts(6);
    for(int i=0;i<6;i++){
      int chk_face = faces[i].internal_intersect(R,ts[i]);
      if(!chk_face) ts[i] = std::numeric_limits<double>::infinity();
    }
    bool hit = false;
    for(int i=0;i<6;i++) if(ts[i] < tmp){
        t = ts[i] < t ? ts[i] : t;
        hit = true;
    }
    return hit;
}

bool Box::internal_get_normal(Point& p, Ray& r){
    // std::vector<bool> on(6,false);
    int s = 0;
    int p_face = -10;
    for(int i=0;i<6;i++){
        if(faces[i].contained(p)){
            // on[i] = true;
            s += 1;
            p_face = i;
            break;
        }
    }
        
    if(s < 1){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point inside/outside the Box");
        }
        return false;
    }
    else if(s > 1){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point on the edge/corner");
        }
        return false;
    }
    // for(int i=0;i<6;i++) if(on[i]) r = Ray(p,faces[i].get_this_normal());
    r = Ray(p,faces[p_face].get_this_normal());
    return true;
}

void Box::print(){
    printf("ID - %d | Type - Box\n",id);
    printf("Verices - \n");
    for(int i=0;i<8;++i) printf("%s\n",vertices[i].toString().c_str());
    printf("Faces - \n");
    for(int i=0;i<6;++i) faces[i].print();
}