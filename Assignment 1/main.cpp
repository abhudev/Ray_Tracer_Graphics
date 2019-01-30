#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>

// #include <cxxopts.hpp>

#include <ctype.h>
#include <stdio.h>
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

void generateScene(std::string filename_, std::vector<Object*>& objs, std::vector<Object*>& lt_srcs, Camera& cam){
    throw std::runtime_error("Not yet implemented");
}

void placeObjects(std::vector<Object*>& objs){
    objs.push_back(new Sphere(Point(0,0,420),100));
    objs.push_back(new Sphere(Point(0,200,215),100));
    objs.push_back(new Plane(Point(-105,0,0), Eigen::Vector3d(1, 0, 0), Color(0,0,100)));
    objs[2]->kd[0] = 0.1; objs[2]->kd[1] = 0.1; objs[2]->kd[2] = 0.1;
    objs[2]->ka[0] = 0.5; objs[2]->ka[1] = 0.5; objs[2]->ka[2] = 0.8;
    objs[2]->ks[0] = 0; objs[2]->ks[1] = 0; objs[2]->ks[2] = 0;
}

void placeLightSrcs(std::vector<LightSrc>& lts){
    lts.push_back(LightSrc(Color(127,127,127),Point(400,200,215)));
    lts.push_back(LightSrc(Color(127,127,127),Point(200,500,250)));
}

void placeCamera(Camera& cam){
    cam = Camera(Point(0,0,-100));
}

void defineVCS(Eigen::Matrix3d& mat, Eigen::Vector3d& r){
    mat << 1, 0, 0, 0, 1, 0, 0, 0, 1;
    r << 0, 0, 5;
}

void defineBackPlane(Plane& bk_plane){
    bk_plane = Plane(Point(0,0,1000),Eigen::Vector3d(0,0,1),Color(100,0,0));
}

int main(int argc, char **argv){

	// :: Command line arguments ::
	// cxxopts::Options options("RayTracer", "Ray casting and recursive ray tracing");
	// options.add_options()
	//   ("w,width", "Image width", cxxopts::value<int>())
	//   ("h,height", "Image height", cxxopts::value<int>())
	//   ("n,super", "Supersampling", cxxopts::value<int>())
	//   ;

	// auto result = options.parse(argc, argv);


    std::vector<Object*> objs;
    placeObjects(objs);
    std::vector<LightSrc> lts;
    placeLightSrcs(lts);
    Camera cam;
    placeCamera(cam);
    Eigen::Matrix3d vcs_mat;
    Eigen::Vector3d vcs_origin;
    defineVCS(vcs_mat,vcs_origin);
    Point cam_wcs(vcs_mat * cam.pt.pt + vcs_origin);
    Plane bk_plane;
    defineBackPlane(bk_plane);
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

    // int width = result["w"].as<int>(), height = result["h"].as<int>(), n = result["n"].as<int>();
    int width = 250, height = 250, n = 2;
    // printf("Width = %d, height = %d, n = %d\n", width, height, n);
    Eigen::MatrixXd img1(width,height), img2(width,height), img3(width,height);
    cast_rays(cam.pt, cam_wcs, vcs_mat, &cam, objs, lts, bk_plane, width, height, n, img1, img2, img3);

    // Recursive!
    // rec_cast_rays(cam.pt, cam_wcs, vcs_mat, &cam, objs, lts, bk_plane, width, height, n, img1, img2, img3);

    std::cout << "Raytracing Done\n";

    std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = width-1; i >= 0; --i) {
        for(int j = 0; j < height; ++j){
            ofs << (unsigned char)(std::min(double(255), img1(i, j))) << (unsigned char)(std::min(double(255), img2(i, j))) << (unsigned char)(std::min(double(255), img3(i, j)));
        }
    }
    ofs.close();
}