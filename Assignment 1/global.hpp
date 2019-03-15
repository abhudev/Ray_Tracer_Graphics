#ifndef __GLOBAL__
#define __GLOBAL__

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

#define PI 3.14159265358979323846

typedef Eigen::Vector3d vec3d;
typedef Eigen::Vector4d vec4d;
typedef Eigen::Matrix3d mat3d;
typedef Eigen::Matrix4d mat4d;
typedef std::pair<vec3d, vec3d> pvv;

extern bool raysim;
extern bool debug;
extern int max_trace_depth;
extern int obj_count;
extern int txt_count;
extern vec3d global_Ia;

const double eps = 1e-6;
const double bias = 1e-4;

std::string toString(vec3d v);
void writePPM(std::string filename, std::vector<Eigen::MatrixXd>& img);
void readPPM(std::string filename, std::vector<Eigen::MatrixXd>& img);

#endif