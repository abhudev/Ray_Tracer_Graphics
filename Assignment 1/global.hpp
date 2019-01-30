#ifndef __GLOBAL__
#define __GLOBAL__

#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <Eigen/Dense>

const int MAX_TRACE_DEPTH = 5;
const double eps = 1e-6;
const double bias = 1e-4;
const Eigen::Vector3d zero_vec(0,0,0);
const Eigen::Vector3d eps_vec(eps,eps,eps);

const Eigen::Vector3d global_Ia(128,128,128);

#endif