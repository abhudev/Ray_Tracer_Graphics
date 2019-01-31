#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <cxxopts.hpp>

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "global.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "raytrace.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "polygon.hpp"
#include "quadric.hpp"
#include "box.hpp"

void generateScene(std::string filename, std::vector<Object*>& objs, std::vector<LightSrc>& lt_srcs, Plane& bk_plane, Camera& cam, Eigen::Matrix3d& vcs_mat, Point& vcs_origin){
    std::ifstream fin(filename);
    json j;
    fin >> j;
    Eigen::Vector3d n(j["camera"]["look_at"][0],j["camera"]["look_at"][1],j["camera"]["look_at"][2]);
    n.normalize();
    cam = Camera(Point((int)(j["camera"]["offset"]) * n));
    vcs_origin = Point(j["vcs"]["origin"][0],j["vcs"]["origin"][1],j["vcs"]["origin"][2]);
    Eigen::Vector3d up(j["vcs"]["up"][0],j["vcs"]["up"][1],j["vcs"]["up"][2]);
    Eigen::Vector3d v = up - up.dot(n)*n;
    v.normalize();
    Eigen::Vector3d u = n.cross(v);
    vcs_mat << u[0], u[1], u[2], v[0], v[1], v[2], n[0], n[1], n[2];
    bk_plane = Plane(Point(vcs_origin.pt + j["background"]["offset"] * n), n, Color(j["background"]["color"][0],j["background"]["color"][1],j["background"]["color"][2]));
    int n_lts = j["lights"]["size"];
    for(int i=0;i<n_lts;i++){
        if(j["lights"][std::string(1,i+'0')].empty()){
            throw std::runtime_error("There are still some light sources to be specified");
        }
        Color rgb(j["lights"][std::string(1,i+'0')]["color"][0],j["lights"][std::string(1,i+'0')]["color"][1],j["lights"][std::string(1,i+'0')]["color"][2]);
        Point loc(j["lights"][std::string(1,i+'0')]["loc"][0],j["lights"][std::string(1,i+'0')]["loc"][1],j["lights"][std::string(1,i+'0')]["loc"][2]);
        lt_srcs.push_back(LightSrc(rgb,loc));
    }
    int n_objs = j["objects"]["size"];
    for(int i=0;i<n_objs;i++){
        json j_obj = j["objects"][std::string(1,i+'0')];
        if(j_obj.empty()){
            throw std::runtime_error("There are still some objects to be specified");
        }
        if(j_obj["name"] == "plane" || j_obj["name"] == "Plane"){
            json j_pt = j_obj["pt"];
            json j_normal = j_obj["normal"];
            objs.push_back(new Plane(Point(j_pt[0],j_pt[1],j_pt[2]), Eigen::Vector3d(j_normal[0],j_normal[1],j_normal[2])));
        }
        else if(j_obj["name"] == "sphere" || j_obj["name"] == "Sphere"){
            json j_pt = j_obj["pt"];
            objs.push_back(new Sphere(Point(j_pt[0],j_pt[1],j_pt[2]),j_obj["radius"]));
        }
        else if(j_obj["name"] == "quadric" || j_obj["name"] == "Quadric"){
            Eigen::Matrix4d mat;
            json j_mat = j_obj["mat"];
            mat << j_mat[0], j_mat[1], j_mat[2], j_mat[3], j_mat[1], j_mat[4], j_mat[5], j_mat[6], j_mat[2], j_mat[5], j_mat[7], j_mat[8], j_mat[3], j_mat[6], j_mat[8], j_mat[9];
            objs.push_back(new Quadric(mat));
        }
        else if(j_obj["name"] == "polygon" || j_obj["name"] == "Polygon"){
            int size = j_obj["sides"];
            std::vector<Point> pts;
            for(int k=0;k<size;k++){
                json j_side = j_obj[std::string(1,k+'0')];
                if(j_obj.empty()){
                    throw std::runtime_error("There are still some sides to be specified");
                }
                pts.push_back(Point(j_side[0],j_side[1],j_side[2]));
            }
            objs.push_back(new Polygon(pts));
        }
        else if(j_obj["name"] == "box" || j_obj["name"] == "Box"){
            json j_a = j_obj["a"];
            json j_b = j_obj["b"];
            json j_c = j_obj["d"];
            json j_d = j_obj["e"];
            objs.push_back(new Box(Point(j_a[0],j_a[1],j_a[2]),Point(j_b[0],j_b[1],j_b[2]),Point(j_c[0],j_c[1],j_c[2]),Point(j_d[0],j_d[1],j_d[2])));
        }
        else{
            throw std::runtime_error("Unsupported object " + (std::string)j_obj["name"]);
        }
        if(!j_obj["reflectivity"].empty())
            objs.back()->reflectivity = j_obj["reflectivity"];
        if(!j_obj["transparency"].empty())
            objs.back()->transparency = j_obj["transparency"];
        if(!j_obj["mu"].empty())
            objs.back()->mu = j_obj["mu"];
        if(!j_obj["s_exp"].empty())
            objs.back()->s_exp = j_obj["s_exp"];
        
        if(!j_obj["kd"].empty()){
            objs.back()->kd = Eigen::Vector3d(j_obj["kd"][0],j_obj["kd"][1],j_obj["kd"][2]);
        }
        if(!j_obj["ks"].empty()){
            objs.back()->ks = Eigen::Vector3d(j_obj["ks"][0],j_obj["ks"][1],j_obj["ks"][2]);
        }
        if(!j_obj["ka"].empty()){
            objs.back()->ka = Eigen::Vector3d(j_obj["ka"][0],j_obj["ka"][1],j_obj["ka"][2]);
        }
    }
}

void placeObjects(std::vector<Object*>& objs){
    objs.push_back(new Sphere(Point(0,0,420),100));
    objs.push_back(new Sphere(Point(0,200,215),100));
    objs.push_back(new Plane(Point(-100,0,0), Eigen::Vector3d(1, 0, 0)));
    objs.push_back(new Plane(Point(1000,0,0), Eigen::Vector3d(-1, 0, 0)));
    objs.push_back(new Plane(Point(0,1000,0), Eigen::Vector3d(0, -1, 0)));
    objs.push_back(new Plane(Point(0,-500,0), Eigen::Vector3d(0, 1, 0)));
    objs.push_back(new Box(Point(0, -100, 100), Point(0, -50, 100), Point(200, -100, 100), Point(0, -100, 150)));    
    for(int i=2;i<6;i++){
        objs[i]->kd[0] = 0.1; objs[i]->kd[1] = 0.1; objs[i]->kd[2] = 0.1;
        objs[i]->ka[0] = 0.5; objs[i]->ka[1] = 0.5; objs[i]->ka[2] = 0.8;
        objs[i]->ks[0] = 0; objs[i]->ks[1] = 0; objs[i]->ks[2] = 0;
    }

    // objs[0]->ka[0] = 0.0215; objs[0]->ka[1] = 0.1745; objs[0]->ka[2] = 0.0215;
    // objs[0]->kd[0] = 0.07568; objs[0]->kd[1] = 0.61424; objs[0]->kd[2] = 0.07568;
    // objs[0]->ks[0] = 0.633; objs[0]->ks[1] = 0.727811; objs[0]->ks[2] = 0.633;

    objs[0]->ka[0] = 0.24725; objs[0]->ka[1] = 0.1995; objs[0]->ka[2] = 0.0745;
    objs[0]->kd[0] = 0.75164; objs[0]->kd[1] = 0.60648; objs[0]->kd[2] = 0.22648;
    objs[0]->ks[0] = 0.628281; objs[0]->ks[1] = 0.555802; objs[0]->ks[2] = 0.366065;

    objs[1]->ka[0] = 0.0215; objs[1]->ka[1] = 0.1745; objs[1]->ka[2] = 0.0215;
    objs[1]->kd[0] = 0.07568; objs[1]->kd[1] = 0.61424; objs[1]->kd[2] = 0.07568;
    objs[1]->ks[0] = 0.633; objs[1]->ks[1] = 0.727811; objs[1]->ks[2] = 0.633;
    
    // objs[1]->ka[0] = 0.24725; objs[1]->ka[1] = 0.1995; objs[1]->ka[2] = 0.0745;
    // objs[1]->kd[0] = 0.75164; objs[1]->kd[1] = 0.60648; objs[1]->kd[2] = 0.22648;
    // objs[1]->ks[0] = 0.628281; objs[1]->ks[1] = 0.555802; objs[1]->ks[2] = 0.366065;

    objs[6]->ka[0] = 0.24725; objs[6]->ka[1] = 0.1995; objs[6]->ka[2] = 0.0745;
    objs[6]->kd[0] = 0.75164; objs[6]->kd[1] = 0.60648; objs[6]->kd[2] = 0.22648;
    objs[6]->ks[0] = 0.628281; objs[6]->ks[1] = 0.555802; objs[6]->ks[2] = 0.366065;
    
    // objs[0]->reflectivity = 0.6;
    objs[0]->reflectivity = 0.6;
    objs[1]->reflectivity = 0.3;
    objs[6]->reflectivity = 0.6;
    objs[0]->transparency = 0;
    objs[1]->transparency = 0.6;
    objs[6]->transparency = 0;
    objs[2]->reflectivity = 0.2;

    objs[0]->s_exp = 30;
    objs[1]->s_exp = 1;
    objs[6]->s_exp = 30;

    objs[0]->mu = 1.5;
    objs[1]->mu = 1.5;
    objs[6]->mu = 1.5;
}

void placeLightSrcs(std::vector<LightSrc>& lts){
    lts.push_back(LightSrc(Color(127,127,127),Point(400,200,215)));
    lts.push_back(LightSrc(Color(127,127,127),Point(200,500,250)));
    // lts.push_back(LightSrc(Color(127,127,127),Point(200,50,0)));
}

void placeCamera(Camera& cam){
    cam = Camera(Point(0,0,-100));
}

void defineVCS(Eigen::Matrix3d& mat, Point& r){
    mat << 1, 0, 0, 0, 1, 0, 0, 0, 1;
    r = Point(0, 0, 95);
}

void defineBackPlane(Plane& bk_plane){
    bk_plane = Plane(Point(0,0,1000),Eigen::Vector3d(0,0,1),Color(100,0,0));
}

int main(int argc, char **argv){

	// Test the objects
	// Box cc(Point(0,0,10), Point(0, 5, 10), Point(20, 0, 10), Point(0, 0, 25));
	// std::vector<Point> pts;
	// pts.push_back(Point(0,0,10));
	// pts.push_back(Point(0,5,10));
	// pts.push_back(Point(20,5,10));
	// pts.push_back(Point(20, 0, 10));
	// Polygon poly(pts);
	// Ray r(Point(0,0,0), Eigen::Vector3d(20, 5, 25));
	// r.rd.normalize();
	// double t;
	// bool chk_intersect = cc.intersect(r, t);

	// Point pt = r.get_pt(t);
	// printf("t: %f\n", t);
	// printf("point: (%f, %f, %f)\n", pt.pt[0], pt.pt[1], pt.pt[2]);
	// printf("chk_intersect: %d\n", chk_intersect);

	// chk_intersect = poly.intersect(r, t);	
	// pt = r.get_pt(t);
	// printf("t: %f\n", t);
	// printf("point: (%f, %f, %f)\n", pt.pt[0], pt.pt[1], pt.pt[2]);
	// printf("chk_intersect: %d\n", chk_intersect);

	// bool chk = poly.contained(pt);
	// std::cout<<"Point in plane: "<<chk<<"\n";
	// chk = poly.contained(Point(10, 2.5, 10));
	// std::cout<<"Point in plane: "<<chk<<"\n";
	// chk = poly.contained(Point(10, 7, 10));
	// std::cout<<"Point in plane: "<<chk<<"\n";

	// Eigen::Vector3d a1(0,0,0), a2(1,1,1);
	// std::cout<<(a1.array()<2)<<"\n";

	// return 0;

	// :: Command line arguments ::
	cxxopts::Options options("RayTracer", "Ray casting and recursive ray tracing");
	options.add_options()
	  ("w,width", "Image width", cxxopts::value<int>())
	  ("h,height", "Image height", cxxopts::value<int>())
	  ("n,super", "Supersampling", cxxopts::value<int>())
	  ;

	auto result = options.parse(argc, argv);

    std::vector<Object*> objs;
    std::vector<LightSrc> lts;
    Camera cam;
    Eigen::Matrix3d vcs_mat;
    Point vcs_origin;
    Plane bk_plane;

    generateScene("scene.json", objs, lts, bk_plane, cam, vcs_mat, vcs_origin);

    // placeObjects(objs);
    // placeLightSrcs(lts);
    // placeCamera(cam);
    // defineVCS(vcs_mat,vcs_origin);
    // defineBackPlane(bk_plane);
    
    Point cam_wcs(vcs_mat * cam.pt.pt + vcs_origin.pt);
    objs.push_back(&bk_plane);
    
    std::cout << "World Defined\n";
    
    // double tmp;
    // Ray r(Point(0,0,1),Eigen::Vector3d(-11,-29,4));
    // bool chk = objs[0]->intersect(r,tmp);

    // std::cout << chk << " - " << tmp << "\n";

    // Ray normal;
    // Point p = r.get_pt(tmp);
    // std::cout << p.pt << "\n";
    // objs[0]->get_normal(p,normal);

    // std::cout << normal.ro.pt << "\n";
    // std::cout << normal.rd << "\n";

    // return 0;

    int width = result["w"].as<int>(), height = result["h"].as<int>(), n = result["n"].as<int>();
    // int width = 500, height = 500, n = result["n"].as<int>();
    // int width = 500, height = 500, n = 2;
    printf("Width = %d, height = %d, n = %d\n", width, height, n);
    Eigen::MatrixXd img1(width,height), img2(width,height), img3(width,height);
    cast_rays(cam.pt, cam_wcs, vcs_mat, &cam, objs, lts, bk_plane, width, height, n, img1, img2, img3);

    // Recursive!
    // rec_cast_rays(cam.pt, cam_wcs, vcs_mat, &cam, objs, lts, bk_plane, width, height, n, img1, img2, img3);

    std::cout << "Raytracing Done\n";

    std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width; ++i) {
        for(int j = 0; j < height; ++j){
            ofs << (unsigned char)(std::min(double(255), img1(i, j))) << (unsigned char)(std::min(double(255), img2(i, j))) << (unsigned char)(std::min(double(255), img3(i, j)));
        }
    }
    ofs.close();
}