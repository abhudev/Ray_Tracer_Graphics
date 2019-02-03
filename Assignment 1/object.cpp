#include "object.hpp"

Object::Object() {}

void Object::printProperties(){
    printf("kd = %s\n",toString(kd));
    printf("ks = %s\n",toString(ks));
    printf("ka = %s\n",toString(ka));
    printf("mu = %f | s_exp = %f | reflectivity = %f | transparency = %f\n",mu, s_exp, reflectivity, transparency);
}