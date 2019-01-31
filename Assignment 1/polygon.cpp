#include "polygon.hpp"

Polygon::Polygon(std::vector<Point>& pts): Object(), vertices(pts) {
    if(vertices.size() < 3) throw std::runtime_error("Polygon requires atleast 3 vertices");
    for(int i=0;i<vertices.size();i++) edges.push_back(vertices[(i+1)%vertices.size()] - vertices[i]);
    Eigen::Vector3d p = vertices[1] - vertices[0];
    Eigen::Vector3d q = vertices[2] - vertices[1];
    normal = (p.cross(q)).normalized();            // Adjust normal according to ray as this is a 2D object
    dist = -1.0*vertices[0].pt.dot(normal);
}

bool Polygon::intersect(Ray& r, double& t){
    double vd = normal.dot(r.rd);
    if(abs(vd) < eps) return false;

    // Handle the case when vd is positive - Refer slides

    double tmp = -1*(normal.dot(r.ro.pt) + dist)/vd;
    // printf("Normal: (%f, %f, %f)\n", normal[0], normal[1], normal[2]);
    // printf("Dist: %f\n", dist);
    // printf("vd: %f\n", vd);
    // printf("")
    if(tmp < 0) return false;    
    Point xpt = r.get_pt(tmp);
    // printf("Vertices:\n");
    // for (int i = 0; i < vertices.size(); ++i)
    // {
    //     std::cout<<vertices[i].pt<<"\n";
    // }
    // printf("xpt (%f, %f, %f), tmp %f\n", xpt.pt[0], xpt.pt[1], xpt.pt[2], tmp);
    // std::cout<<vertices<<"\n\n";
    if(!contained(xpt)) return false;
    t = tmp;
    return true;
}

bool Polygon::contained(const Point& p){
    if(abs(normal.dot(vertices[0] - p)) > eps) 
        {
            // printf("Point not on polygon\n");
            // printf("Normal (%f, %f. %f)\n", normal[0], normal[1], normal[2]);
            // printf("Vertices:\n");
            // for (int i = 0; i < vertices.size(); ++i)
            // {
            //     std::cout<<vertices[i].pt<<"\n";
            // }
            // printf("Point:\n");
            // std::cout<<p.pt<<"\n";

            // throw std::runtime_error("Point not on the polygon");
            return false;
        }

    // Check - is point one of the vertices?
    // for(int i = 0; i < vertices.size(); ++i)
    // {
    //     int c0 = abs(p.pt[0] - vertices[i].pt[0]) < eps;
    //     int c1 = abs(p.pt[1] - vertices[i].pt[1]) < eps;
    //     int c2 = abs(p.pt[2] - vertices[i].pt[2]) < eps;

    // }

    bool not_valid = true;
    Eigen::Vector3d zero_vec(0,0,0), dir;
    int cc_i = 0;
    while(not_valid){
        Point some_pt(rand()%100,rand()%100,rand()%100);
        Eigen::Vector3d projected_pt = some_pt.pt - (some_pt.pt - p.pt).dot(normal) * normal;
        dir = (projected_pt - p.pt).normalized();
        // printf("dir (%f, %f, %f)\n", dir[0], dir[1], dir[2]);
        // double check_dot = dir.dot(normal);
        // printf("Check dot %f\n", check_dot);
        not_valid = false;

        // Check if the ray passes through any vertices
        for(int i=0;i<vertices.size() && !not_valid;i++){
          // not_valid |= dir.cross(vertices[i] - p) == zero_vec;
          Eigen::Vector3d cross_vec = dir.cross(vertices[i] - p);
          Eigen::Vector3i check_vec;
          check_vec[0] = abs(cross_vec[0]) < eps;
          check_vec[1] = abs(cross_vec[1]) < eps;
          check_vec[2] = abs(cross_vec[2]) < eps;
          not_valid |= (check_vec[0] && check_vec[1] && check_vec[2]);
        }
        cc_i++;
        if(cc_i >= 3)
        {
            return true;
        }
        // if(!not_valid) continue;        
    }

    int count = 0;
    for(int i=0;i<vertices.size();i++){
        Eigen::Vector3d prod = edges[i].cross(dir);
        // printf("Edge %d - (%f, %f, %f) and dir (%f, %f, %f)\n", i, edges[i][0], edges[i][1], edges[i][2], dir[0], dir[1], dir[2]);
        double pnorm = prod.squaredNorm();
        double t1 = ((p - vertices[i]).cross(dir)).dot(prod)/pnorm;
        double t2 = -1*(vertices[i] - p).cross(edges[i]).dot(prod)/pnorm;
        // printf("t1 %f t2 %f\n", t1, t2);
        if(t1 > 0 && t1 < 1 && t2 >= 0) count++;            // t2 == 0 - Point on the edge case
    }
    // printf("Count: %d\n", count);
    return count%2 == 1;
}

bool Polygon::get_normal(Point& p, Ray& r){
    if(!contained(p))
    {
        // throw std::runtime_error("Point not inside the polygon");
        return false;
    }
    r = Ray(p,normal);
    return true;
}

Eigen::Vector3d Polygon::get_this_normal(){
    return normal;
}