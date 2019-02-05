#include "global.hpp"

bool debug = false;
int max_trace_depth = 5;
int obj_count = 0;
vec3d global_Ia = vec3d(128,128,128);

std::string toString(vec3d v){
    std::stringstream ss;
    ss << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    return ss.str();
}

void writePPM(std::string filename, std::vector<Eigen::MatrixXd>& img){
    int height = img[0].rows(), width = img[0].cols();
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j){
            ofs << (unsigned char)(std::min(double(255), img[0](i, j))) << (unsigned char)(std::min(double(255), img[1](i, j))) << (unsigned char)(std::min(double(255), img[2](i, j)));
        }
    }
    ofs.close();
}