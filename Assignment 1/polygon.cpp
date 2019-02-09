#include "polygon.hpp"

Polygon::Polygon(std::vector<Point>& pts): Object(), id(obj_count++), vertices(pts) {
    if(vertices.size() < 3) throw std::runtime_error("Polygon requires atleast 3 vertices");
    for(uint i=0;i<vertices.size();i++) edges.push_back(vertices[(i+1)%vertices.size()] - vertices[i]);
    vec3d p = vertices[1] - vertices[0];
    vec3d q = vertices[2] - vertices[1];
    normal = (p.cross(q)).normalized();            // Adjust normal according to ray as this is a 2D object
    dist = -1.0*vertices[0].pt.dot(normal);
}

bool Polygon::internal_intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(abs(vd) < eps) return false;

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    if(tmp < 0) return false;    
    Point xpt = r.get_pt(tmp);
    if(!contained(xpt)) return false;
    t = tmp;
    return true;
}

bool Polygon::contained(Point& p){
    if(abs(normal.dot(vertices[0] - p)) > eps) {
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not on the plane of polygon");
        }
        return false;
    }

    bool not_valid = true;
    vec3d zero_vec(0,0,0), dir;
    int cc_i = 0;
    while(not_valid){        
        Point some_pt(rand()%100,rand()%100,rand()%100);
        vec3d projected_pt = some_pt.pt - (some_pt.pt - p.pt).dot(normal) * normal;
        dir = (projected_pt - p.pt).normalized();
        not_valid = false;

        // Check if the ray passes through any vertices
        for(uint i=0;i<vertices.size() && !not_valid;i++){
          vec3d cross_vec = dir.cross(vertices[i] - p);
          Eigen::Vector3i check_vec;
          check_vec[0] = abs(cross_vec[0]) < eps;
          check_vec[1] = abs(cross_vec[1]) < eps;
          check_vec[2] = abs(cross_vec[2]) < eps;
          not_valid |= (check_vec[0] && check_vec[1] && check_vec[2]);
        }
        cc_i++;
        if(cc_i >= 3){
            return true;
        }
    }

    int count = 0;
    for(uint i=0;i<vertices.size();i++){
        vec3d prod = edges[i].cross(dir);
        double pnorm = prod.squaredNorm();
        double t1 = ((p - vertices[i]).cross(dir)).dot(prod)/pnorm;
        double t2 = -1*(vertices[i] - p).cross(edges[i]).dot(prod)/pnorm;
        if(t1 > 0 && t1 < 1 && t2 >= 0) count++;            // t2 == 0 - Point on the edge case
    }
    return count%2 == 1;
}

bool Polygon::internal_get_normal(Point& p, Ray& r){
    if(!contained(p)){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not inside the polygon");
        }
        return false;
    }
    r = Ray(p,normal);
    return true;
}

vec3d Polygon::get_this_normal(){
    return normal;
}

void Polygon::print(){
    printf("ID - %d | Type - Polygon\n",id);
    printf("n: %s\n",toString(normal).c_str());
    printf("d: %f\n",dist);
    printf("Points - \n");
    for(uint i=0;i<vertices.size();++i) printf("%s\n",vertices[i].toString().c_str());
}