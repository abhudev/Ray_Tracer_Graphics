#include "object.hpp"

Object::Object() {}

void Object::printProperties(){
    printf("kd = %s\n",toString(kd).c_str());
    printf("ks = %s\n",toString(ks).c_str());
    printf("ka = %s\n",toString(ka).c_str());
    printf("mu = %f | s_exp = %f | reflectivity = %f | transparency = %f\n",mu, s_exp, reflectivity, transparency);
}