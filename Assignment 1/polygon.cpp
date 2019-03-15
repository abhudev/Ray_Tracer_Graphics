#include "polygon.hpp"
#include "earcut.hpp"

Polygon::Polygon(std::vector<Point>& pts): Object("Polygon"), id(obj_count++), vertices(pts) {
    if(vertices.size() < 3) throw std::runtime_error("Polygon requires atleast 3 vertices");
    for(uint i=0;i<vertices.size();i++) edges.push_back(vertices[(i+1)%vertices.size()] - vertices[i]);
    vec3d p = vertices[1] - vertices[0];
    vec3d q = vertices[2] - vertices[1];
    normal = (p.cross(q)).normalized();            // Adjust normal according to ray as this is a 2D object
    dist = -1.0*vertices[0].pt.dot(normal);
}

bool Polygon::internal_intersect(Ray& r, double& t, int& args){
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

bool Polygon::internal_get_normal(Point& p, Ray& r, int& args){
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

double Polygon::get_dist(){
    return dist;
}

vec3d Polygon::get_normal(){
    return normal;
}

void Polygon::print(){
    printf("ID - %d | Type - Polygon\n",id);
    printf("n: %s\n",toString(normal).c_str());
    printf("d: %f\n",dist);
    printf("Points - \n");
    for(uint i=0;i<vertices.size();++i) printf("%s\n",vertices[i].toString().c_str());
}

bool Polygon::get_mesh(std::ofstream& fout) {
 
    std::vector<std::pair<double, double> > proj_vx;
    std::vector<std::pair<double, double> > proj_vy;
    std::vector<std::pair<double, double> > proj_vz;
    for (uint i = 0; i < vertices.size(); ++i) {
        // Project onto z=0
        // printf("POLY (%f,%f,%f)\n", vertices[i].pt[0], vertices[i].pt[1], vertices[i].pt[2]);
        std::pair<double, double> ppx(vertices[i].pt[1], vertices[i].pt[2]);
        std::pair<double, double> ppy(vertices[i].pt[0], vertices[i].pt[2]);
        std::pair<double, double> ppz(vertices[i].pt[0], vertices[i].pt[1]);
        proj_vx.push_back(ppx);
        proj_vy.push_back(ppy);
        proj_vz.push_back(ppz);
    }
    
    // Create array for polygon
    std::vector<std::vector<std::pair<double, double> > > polygonx;
    std::vector<std::vector<std::pair<double, double> > > polygony;
    std::vector<std::vector<std::pair<double, double> > > polygonz;
    polygonx.push_back(proj_vx);
    polygony.push_back(proj_vy);
    polygonz.push_back(proj_vz);

    std::vector<int> indicesx = mapbox::earcut<int>(polygonx);
    std::vector<int> indicesy = mapbox::earcut<int>(polygony);
    std::vector<int> indicesz = mapbox::earcut<int>(polygonz);

    std::vector<int> indices;

    int sx = indicesx.size();
    int sy = indicesy.size();
    int sz = indicesz.size();

    if(sx >= sy && sx >= sz) {
        indices = indicesx;
    }

    else if(sy >= sx && sy >= sz) {
        indices = indicesy;   
    }

    else {
        indices = indicesz;
    }


    // print
    // printf("Number of Triangles: %d\n", indices.size());

    // Now print to file
    for (uint i = 0; i < indices.size(); ++i) {
        vec3d p1(vertices[indices[i]].pt);
        if(transform) p1 = M*p1 + d0;
        fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
    }

    for (uint i = 0; i < indices.size(); ++i)
        fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";    

    return true;
}