#include "global.hpp"

bool raysim = false;
bool debug = false;
int max_trace_depth = 5;
int obj_count = 0;
int txt_count = 0;
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
    for (int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            ofs << (unsigned char)(std::min(double(255), img[0](i, j))) << (unsigned char)(std::min(double(255), img[1](i, j))) << (unsigned char)(std::min(double(255), img[2](i, j)));
        }
    }
    ofs.close();
}

void readPPM(std::string filename, std::vector<Eigen::MatrixXd>& img){
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    if(ifs.fail()) throw std::runtime_error("Can't open input file");

    std::string header;
    int w, h, b;
    ifs >> header;
    if (strcmp(header.c_str(), "P6") != 0) throw std::runtime_error("Wrong input format (!= P6)");
    ifs >> w >> h >> b;
    for(int i=0;i<3;++i) img.push_back(Eigen::MatrixXd(h,w));

    ifs.ignore(256,'\n');
    unsigned char pix[3];
    for (int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            ifs.read(reinterpret_cast<char *>(pix), 3);
            img[0](i, j) = pix[0];
            img[1](i, j) = pix[1];
            img[2](i, j) = pix[2];
        }
    }
    ifs.close();
}