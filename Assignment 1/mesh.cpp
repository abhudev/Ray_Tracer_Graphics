#include "mesh.hpp"

Mesh::Mesh(std::vector<Triangle>& ts) : Object("Mesh"), id(obj_count++), faces(ts) {
    double x[2], y[2], z[2];
    x[0] = std::numeric_limits<double>::max(); x[1] = std::numeric_limits<double>::min();
    y[0] = std::numeric_limits<double>::max(); y[1] = std::numeric_limits<double>::min();
    z[0] = std::numeric_limits<double>::max(); z[1] = std::numeric_limits<double>::min();
    for(uint i=0;i<faces.size();++i){
        for(int j=0;j<3;++j){
            if(faces[i].vertices[j].pt(0) < x[0]) x[0] = faces[i].vertices[j].pt(0);
            if(faces[i].vertices[j].pt(0) > x[1]) x[1] = faces[i].vertices[j].pt(0);
            if(faces[i].vertices[j].pt(1) < y[0]) y[0] = faces[i].vertices[j].pt(1);
            if(faces[i].vertices[j].pt(1) > y[1]) y[1] = faces[i].vertices[j].pt(1);
            if(faces[i].vertices[j].pt(2) < z[0]) z[0] = faces[i].vertices[j].pt(2);
            if(faces[i].vertices[j].pt(2) > z[1]) z[1] = faces[i].vertices[j].pt(2);
        }
    }
    obj_count--;
    Point a(x[0],y[1],z[0]);
    Point b(x[1],y[1],z[0]);
    Point d(x[0],y[0],z[0]);
    Point e(x[0],y[1],z[1]);
    boundingBox = Box(a,b,d,e);
}

Mesh::Mesh(std::string fname) : Object(), id(obj_count++){
    readObjFile(fname, faces);
}

void Mesh::readObjFile(std::string fname, std::vector<Triangle>& ts){
    std::ifstream ifs;
    ifs.open(fname, std::ifstream::in);
    if(ifs.fail()) throw std::runtime_error("Can't open input file");

    std::string line;
    std::vector<Point> pts;
    while(getline(ifs, line)){
        if(line.front() == 'v'){
            std::istringstream iss(line);
            char c;
            double p[3];
            iss >> c >> p[0]; iss >> p[1]; iss >> p[2];
            Point pt(vec3d(p[0],p[1],p[2]));
            pts.push_back(pt);
        }
        else if(line.front() == 'f'){
            std::istringstream iss(line);
            char c;
            int p[3];
            iss >> c >> p[0]; iss >> p[1]; iss >> p[2];
            obj_count--;
            ts.push_back(Triangle(pts[p[0]-1],pts[p[1]-1],pts[p[2]-1]));
        }
    }
    double x[2], y[2], z[2];
    x[0] = std::numeric_limits<double>::max(); x[1] = std::numeric_limits<double>::min();
    y[0] = std::numeric_limits<double>::max(); y[1] = std::numeric_limits<double>::min();
    z[0] = std::numeric_limits<double>::max(); z[1] = std::numeric_limits<double>::min();
    for(uint i=0;i<pts.size();++i){
        if(pts[i].pt(0) < x[0]) x[0] = pts[i].pt(0);
        if(pts[i].pt(0) > x[1]) x[1] = pts[i].pt(0);
        if(pts[i].pt(1) < y[0]) y[0] = pts[i].pt(1);
        if(pts[i].pt(1) > y[1]) y[1] = pts[i].pt(1);
        if(pts[i].pt(2) < z[0]) z[0] = pts[i].pt(2);
        if(pts[i].pt(2) > z[1]) z[1] = pts[i].pt(2);
    }
    obj_count--;
    Point a(x[0],y[1],z[0]);
    Point b(x[1],y[1],z[0]);
    Point d(x[0],y[0],z[0]);
    Point e(x[0],y[1],z[1]);
    boundingBox = Box(a,b,d,e);
}

bool Mesh::internal_intersect(Ray& r, double& t, int& args){
    t = std::numeric_limits<double>::infinity();
    int dummy;
    if(!boundingBox.internal_intersect(r,t,dummy)) return false;
    t = std::numeric_limits<double>::infinity();
    double tmp = t;
    std::vector<double> ts(faces.size());
    for(uint i=0;i<faces.size();i++){
      int chk_face = faces[i].internal_intersect(r,ts[i],dummy);
      if(!chk_face) ts[i] = std::numeric_limits<double>::infinity();
    }
    bool hit = false;
    for(uint i=0;i<faces.size();i++) if(ts[i] < tmp){
        if(ts[i] < t){
        	t = ts[i];
        	args = i;
        }
        hit = true;
    }
    return hit;
}

bool Mesh::internal_get_normal(Point& p, Ray& r, int& args){
    if(!debug){
		r = Ray(p,faces[args].get_normal());
	}
	else{
	    int s = 0;
	    int p_face = -1;
	    for(uint i=0;i<faces.size();i++){
	        if(faces[i].contained(p)){
	            s += 1;
	            p_face = i;
	            break;
	        }
	    }
	    if(s < 1){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point inside/outside the Mesh");
	    }
	    else if(s > 1){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point on the edge/corner of the Mesh");
	    }
	    r = Ray(p,faces[p_face].get_normal());
	}
    return true;
}

std::vector<Triangle> Mesh::get_faces(){
	return faces;
}

void Mesh::print(){
    printf("ID - %d | Type - Mesh\n",id);
    printf("Faces (%d) - \n",(int)faces.size());
    for(uint i=0;i<faces.size();++i) faces[i].print();
}

bool Mesh::get_mesh(std::ofstream& fout){
    for(uint i=0;i<faces.size();++i){
    	for(int j=0;j<3;++j){
    		vec3d p(faces[i].vertices[j].pt);
    		if(transform) p = M*p + d0;
    		fout << "v " << p[0] << p[1] << p[2];
    	}
    }
    for(uint i=0;i<faces.size();++i){
    	for(int j=0;j<3;++j){
    		fout << "c " << ka[0] << ka[1] << ka[2];
    	}
    }
    return true;
}