#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <cxxopts.hpp>

#include "global.hpp"
#include "scene.hpp"
#include "raytrace.hpp"
#include "spherical.hpp"

int main(int argc, char **argv){
    // :: Command line arguments ::
    cxxopts::Options options("RayTracer", "Ray casting and recursive ray tracing");
    options.add_options()
        ("h, height", "Image height", cxxopts::value<int>()->default_value("500"))
        ("w, width", "Image width", cxxopts::value<int>()->default_value("500"))        
        ("r, rows", "Number of rows", cxxopts::value<int>()->default_value("500"))
        ("c, cols", "Number of columns", cxxopts::value<int>()->default_value("500"))
        ("n, super", "Supersampling", cxxopts::value<int>()->default_value("1"))        
        ("i, scene", "Input scene (JSON)", cxxopts::value<std::string>())
        ("o, image", "Output image (PPM)", cxxopts::value<std::string>())
        ("b, obj", "Output object file (ob)", cxxopts::value<std::string>())
        ("m, max_depth", "Trace depth", cxxopts::value<int>()->default_value("3"))
        ("d, debug", "Debug flag", cxxopts::value<bool>()->implicit_value("true"))
        ("g, openglRays", "OpenGL simulation", cxxopts::value<bool>()->implicit_value("true"));

    auto result = options.parse(argc, argv);

    max_trace_depth = result["m"].as<int>();
    debug = result["d"].as<bool>();

    Scene scn(result["i"].as<std::string>());
    printf("World Defined\n");

    int width = result["w"].as<int>(), height = result["h"].as<int>(), n = result["n"].as<int>();
    int rows = result["r"].as<int>(), cols = result["c"].as<int>();
    printf("Height = %d, Width = %d, n = %d, Max Depth=%d\n", height, width, n, max_trace_depth);
    printf("Rows = %d, Columns = %d\n", rows, cols);
    
    std::vector<Eigen::MatrixXd> img(3,Eigen::MatrixXd(rows,cols));

    // :: Store rays ::
    std::vector<std::vector<std::vector<pvv> > > ray_structure;
    
    raysim = result["g"].as<bool>();
    printf("raysim = %d\n", raysim);

    if(raysim)
    {
        printf("Ray tracing simulation requested, defaulting to supersampling level = 1\n");
        cast_rays(scn, width, height, cols, rows, 1, img, ray_structure);
    }
    else
        cast_rays(scn, width, height, cols, rows, n, img, ray_structure);
    printf("Raytracing Done\n");

    std::string outfile = result["o"].as<std::string>();
    writePPM(outfile, img);

    if(!result["g"].as<bool>())
        return 0;

    // Now write to the ob file
    std::ofstream fout(result["b"].as<std::string>(), std::ios::out);    

    // Rays
    for (uint i = 0; i < ray_structure.size(); ++i)
        for (uint j = 0; j < ray_structure[0].size(); ++j)
            for(auto line: ray_structure[i][j]) {
                vec3d p1 = line.first;
                vec3d p2 = line.second;
                fout<<"ray "<<i<<" "<<j<<" "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<" "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<"\n";
            }

    // Image plane
    for (int K = 0; K < 2; ++K)                
        for (int i = 0; i < height - 1; ++i)        
            for (int j = 0; j < width - 1; ++j)
            {            
                if(K == 0) {
                    vec3d diff(height/2.0, width/2.0, 0);
                    vec3d p1(i,j,0);
                    vec3d p2(i,j+1,0);
                    vec3d p3(i+1,j+1,0);
                    vec3d p4(i+1, j, 0);                        

                    p1 = p1 - diff;
                    p2 = p2 - diff;
                    p3 = p3 - diff;
                    p4 = p4 - diff;

                    p1 = scn.vcs_mat * p1 + scn.vcs_origin.pt;
                    p2 = scn.vcs_mat * p2 + scn.vcs_origin.pt;
                    p3 = scn.vcs_mat * p3 + scn.vcs_origin.pt;
                    p4 = scn.vcs_mat * p4 + scn.vcs_origin.pt;
                    
                    fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
                    fout<<"v "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<"\n";
                    fout<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<"\n";        
                    fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";
                    fout<<"v "<<p4[0]<<" "<<p4[1]<<" "<<p4[2]<<"\n";
                    fout<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<"\n";
                }                
            
                else {
                    fout<<"c "<<img[0](i,j)/255.0<<" "<<img[1](i,j)/255.0<<" "<<img[2](i,j)/255.0<<"\n";
                    fout<<"c "<<img[0](i,j+1)/255.0<<" "<<img[1](i,j+1)/255.0<<" "<<img[2](i,j+1)/255.0<<"\n";
                    fout<<"c "<<img[0](i+1,j+1)/255.0<<" "<<img[1](i+1,j+1)/255.0<<" "<<img[2](i+1,j+1)/255.0<<"\n";
                    fout<<"c "<<img[0](i,j)/255.0<<" "<<img[1](i,j)/255.0<<" "<<img[2](i,j)/255.0<<"\n";
                    fout<<"c "<<img[0](i+1,j)/255.0<<" "<<img[1](i+1,j)/255.0<<" "<<img[2](i+1,j)/255.0<<"\n";
                    fout<<"c "<<img[0](i+1,j+1)/255.0<<" "<<img[1](i+1,j+1)/255.0<<" "<<img[2](i+1,j+1)/255.0<<"\n";
                }                
            }        

    // Light sources
    for (auto lt_src: scn.lt_srcs) lt_src.get_mesh(fout);

    // Camera
    // scn.cam.get_mesh(fout, scn.cam_wcs);
    std::vector<vec3d> vrt;
    std::vector<vec3d> clr;
    std::vector<int> vind;
    vrt.push_back (vec3d(0+scn.cam_wcs.pt[0], 0+scn.cam_wcs.pt[1], 1+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(0+scn.cam_wcs.pt[0], 0+scn.cam_wcs.pt[1], -1+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(0+scn.cam_wcs.pt[0], 1+scn.cam_wcs.pt[1], 0+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(1+scn.cam_wcs.pt[0], 0+scn.cam_wcs.pt[1], 0+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(0+scn.cam_wcs.pt[0], -1+scn.cam_wcs.pt[1], 0+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(-1+scn.cam_wcs.pt[0], 0+scn.cam_wcs.pt[1], 0+scn.cam_wcs.pt[2])); clr.push_back(vec3d(1,1,0));

    vind.push_back(3); vind.push_back(2); vind.push_back(0); // 3 2 0
    vind.push_back(2); vind.push_back(5); vind.push_back(0); // 2 5 0
    vind.push_back(5); vind.push_back(4); vind.push_back(0); // 5 4 0
    vind.push_back(4); vind.push_back(3); vind.push_back(0); // 4 3 0
    vind.push_back(3); vind.push_back(2); vind.push_back(1); // 3 2 1
    vind.push_back(2); vind.push_back(5); vind.push_back(1); // 2 5 1
    vind.push_back(5); vind.push_back(4); vind.push_back(1); // 5 4 1
    vind.push_back(4); vind.push_back(3); vind.push_back(1); // 4 3 1
    
    for (int i = 0; i < 24; i+=3) {
        fout<<"v "<<vrt[vind[i]][0]<<" "<<vrt[vind[i]][1]<<" "<<vrt[vind[i]][2]<<"\n";
        fout<<"v "<<vrt[vind[i+1]][0]<<" "<<vrt[vind[i+1]][1]<<" "<<vrt[vind[i+1]][2]<<"\n";
        fout<<"v "<<vrt[vind[i+2]][0]<<" "<<vrt[vind[i+2]][1]<<" "<<vrt[vind[i+2]][2]<<"\n";
    }
    
    for (int i = 0; i < 24; i+=3) {
        fout<<"c "<<clr[vind[i]][0]<<" "<<clr[vind[i]][1]<<" "<<clr[vind[i]][2]<<"\n";
        fout<<"c "<<clr[vind[i+1]][0]<<" "<<clr[vind[i+1]][1]<<" "<<clr[vind[i+1]][2]<<"\n";
        fout<<"c "<<clr[vind[i+2]][0]<<" "<<clr[vind[i+2]][1]<<" "<<clr[vind[i+2]][2]<<"\n";            
    }

    // Objects
    for(auto object: scn.objs)
        object->get_mesh(fout);

    // Light rays?

    fout<<"v -1000 -1000 -1000";

    fout.close();
}