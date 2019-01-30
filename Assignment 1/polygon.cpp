#include "polygon.hpp"

Polygon::Polygon(std::vector<Point>& pts): Object(), vertices(pts) {
    if(vertices.size() < 3) throw std::runtime_error("Polygon requires atleast 3 vertices");
    for(int i=0;i<vertices.size();i++) edges.push_back(vertices[(i+1)%vertices.size()] - vertices[i]);
    Eigen::Vector3d p = vertices[1] - vertices[0];
    Eigen::Vector3d q = vertices[2] - vertices[1];
    normal = (p.cross(q)).normalized();            // Adjust normal according to ray as this is a 2D object
    dist = abs(vertices[0].pt.dot(normal));
}

bool Polygon::intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(vd == 0) return false;

    // Handle the case when vd is positive - Refer slides

    t = abs(-1*(normal.dot(r.ro.pt) + dist)/vd);
    return true;
}

bool Polygon::contained(const Point& p){
    if(normal.dot(vertices[0] - p) != 0) throw std::runtime_error("Point not on the plane");

    bool not_valid = true;
    Eigen::Vector3d zero_vec(0,0,0), dir;
    while(not_valid){
        Point some_pt(rand()%100,rand()%100,rand()%100);
        dir = (-1*normal - (p - some_pt)).normalized();
        not_valid = false;

        // Check if the ray passes through any vertices
        for(int i=0;i<vertices.size() && !not_valid;i++) not_valid |= dir.cross(vertices[i] - p) == zero_vec;
        if(!not_valid) continue;

        // Check if the ray coincides with any of the edges
        for(int i=0;i<vertices.size() && !not_valid;i++) not_valid |= (dir.cross(edges[i]) == zero_vec && dir.cross(vertices[i] - p) == zero_vec);
    }

    int count = 0;
    for(int i=0;i<vertices.size();i++){
        Eigen::Vector3d prod = edges[i].cross(dir);
        int t1 = ((p - vertices[i]).cross(dir)).dot(prod)/prod.squaredNorm();
        int t2 = -1*(vertices[i] - p).cross(edges[i]).dot(prod)/prod.squaredNorm();
        if(t1 > 0 && t1 < 1 && t2 >= 0) count++;            // t2 == 0 - Point on the edge case
    }
    return count%2 == 1;
}

void Polygon::get_normal(Point& p, Ray& r){
    if(contained(p)) throw std::runtime_error("Point not inside the polygon");
    r = Ray(p,normal);
}

Eigen::Vector3d Polygon::get_this_normal(){
    return normal;
}