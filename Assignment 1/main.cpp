#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <cxxopts.hpp>

#include "global.hpp"
#include "scene.hpp"
#include "raytrace.hpp"

int main(int argc, char **argv){
	// :: Command line arguments ::
	cxxopts::Options options("RayTracer", "Ray casting and recursive ray tracing");
	options.add_options()
        ("w, width", "Image width", cxxopts::value<int>()->default_value("500"))
        ("h, height", "Image height", cxxopts::value<int>()->default_value("500"))
        ("n, super", "Supersampling", cxxopts::value<int>()->default_value("1"))        
        ("i, scene", "Input scene (JSON)", cxxopts::value<std::string>())
        ("o, image", "Output image (PPM)", cxxopts::value<std::string>())
        ("m, max_depth", "Trace depth", cxxopts::value<int>()->default_value("3"))
        ("d, debug", "Debug flag", cxxopts::value<bool>()->implicit_value("true"));

	auto result = options.parse(argc, argv);

    max_trace_depth = result["m"].as<int>();
    debug = result["d"].as<bool>();

    Scene scn(result["i"].as<std::string>());
    printf("World Defined\n");

    int width = result["w"].as<int>(), height = result["h"].as<int>(), n = result["n"].as<int>();
    printf("Width = %d, Height = %d, n = %d, Max Depth=%d\n", width, height, n, max_trace_depth);
    
    std::vector<Eigen::MatrixXd> img(3,Eigen::MatrixXd(height,width));
        
    cast_rays(scn, width, height, n, img);
    printf("Raytracing Done\n");

    std::string outfile = result["o"].as<std::string>();
    writePPM(outfile, img);
}