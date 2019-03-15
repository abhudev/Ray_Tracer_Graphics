#include "box.hpp"

Box::Box() : Object("Box") {}

Box::Box(const Point& p1, const Point& p2, const Point& p3, const Point& p4): Object("Box"), id(obj_count++) {
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

bool Box::internal_intersect(Ray& r, double& t, int& args){
    t = std::numeric_limits<double>::infinity();
    double tmp = t;
    int dummy;
    std::vector<double> ts(6);
    for(int i=0;i<6;i++){
      int chk_face = faces[i].internal_intersect(r,ts[i],dummy);
      if(!chk_face) ts[i] = std::numeric_limits<double>::infinity();
    }
    bool hit = false;
    for(int i=0;i<6;i++) if(ts[i] < tmp){
        if(ts[i] < t){
        	t = ts[i];
        	args = i;
        }
        hit = true;
    }
    return hit;
}

bool Box::internal_get_normal(Point& p, Ray& r, int& args){
	if(!debug){
		r = Ray(p,faces[args].get_normal());
	}
	else{
	    int s = 0;
	    int p_face = -1;
	    for(int i=0;i<6;i++){
	        // if(faces[i].contained(p)){
	        //     s += 1;
	        //     p_face = i;
	        //     break;
	        // }
	        if(abs(faces[i].get_normal().dot(p.pt) + faces[i].get_dist()) < eps){
	            bool fail = false;
	            for(int j=0;j<6;++j){
	                if(j == i) continue;
	                if(abs(faces[j].get_normal().dot(p.pt) + faces[j].get_dist()) > eps){
	                    fail = true;
	                    break;
	                }
	            }
	            if(!fail){
	                s += 1;
	                p_face = i;
	                break;
	            }
	        }
	    }
	    if(s < 1){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point inside/outside the Box");
	    }
	    else if(s > 1){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point on the edge/corner of the Box");
	    }
	    r = Ray(p,faces[p_face].get_normal());
	}
    return true;
}

std::vector<Polygon> Box::get_faces(){
	return faces;
}

void Box::print(){
    printf("ID - %d | Type - Box\n",id);
    printf("Vertices - \n");
    for(int i=0;i<8;++i) printf("%s\n",vertices[i].toString().c_str());
    printf("Faces - \n");
    for(int i=0;i<6;++i) faces[i].print();
}

bool Box::get_mesh(std::ofstream& fout)
{
    // Write 12 triangles to file

    std::vector<int> vrts;                                                

    vrts.push_back(0); vrts.push_back(1); vrts.push_back(5);    // 0 5 1
    vrts.push_back(0); vrts.push_back(4); vrts.push_back(5);    // 0 5 4
    vrts.push_back(1); vrts.push_back(6); vrts.push_back(2);    // 1 6 2
    vrts.push_back(1); vrts.push_back(6); vrts.push_back(5);    // 1 6 5
    vrts.push_back(2); vrts.push_back(7); vrts.push_back(6);    // 2 7 6
    vrts.push_back(2); vrts.push_back(7); vrts.push_back(3);    // 2 7 3
    vrts.push_back(0); vrts.push_back(7); vrts.push_back(3);    // 0 7 3
    vrts.push_back(0); vrts.push_back(7); vrts.push_back(4);    // 0 7 4
    vrts.push_back(0); vrts.push_back(2); vrts.push_back(1);    // 0 2 1
    vrts.push_back(0); vrts.push_back(2); vrts.push_back(3);    // 0 2 3
    vrts.push_back(4); vrts.push_back(6); vrts.push_back(5);    // 4 6 5
    vrts.push_back(4); vrts.push_back(6); vrts.push_back(7);    // 4 6 7

    int i = 0;
    while(i < 36){
        vec3d p1(vertices[vrts[i]].pt), p2(vertices[vrts[i+1]].pt), p3(vertices[vrts[i+2]].pt);

        if(transform) {
            p1 = M*p1 + d0;
            p2 = M*p2 + d0;
            p3 = M*p3 + d0;
        }

        fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
        fout<<"v "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<"\n";
        fout<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<"\n";
        i += 3;
    }

    i = 0;
    while(i < 36){
        fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
        fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
        fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";
        i += 3;
    }

    return true;
}