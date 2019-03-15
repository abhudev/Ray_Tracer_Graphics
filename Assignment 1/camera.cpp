#include "camera.hpp"

Camera::Camera() : Object("Camera"), pt(), look_at() {}

Camera::Camera(const Point& p, const vec3d l) : Object("Camera"), pt(p), look_at(l) {}

bool Camera::internal_intersect(Ray& r, double& t, int& args){
    throw std::runtime_error("Camera object does not implement internal_intersect");
}

bool Camera::internal_get_normal(Point& p, Ray& r, int& args){
    throw std::runtime_error("Camera object does not implement internal_get_normal");
}

void Camera::print(){
    printf("Type - Camera\n");
    printf("loc: %s\n",pt.toString().c_str());
    printf("look_at: %s\n",toString(look_at).c_str());
}

bool Camera::get_mesh(std::ofstream& fout)
{
	std::vector<vec3d> vrt;
    std::vector<vec3d> clr;
    std::vector<int> vind;
    vrt.push_back (vec3d(0+pt.pt[0], 0+pt.pt[1], 1+pt.pt[2])); clr.push_back(vec3d(1,0,0));
    vrt.push_back (vec3d(0+pt.pt[0], 0+pt.pt[1], -1+pt.pt[2])); clr.push_back(vec3d(0,1,0));
    vrt.push_back (vec3d(0+pt.pt[0], 1+pt.pt[1], 0+pt.pt[2])); clr.push_back(vec3d(0,0,1));
    vrt.push_back (vec3d(1+pt.pt[0], 0+pt.pt[1], 0+pt.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(0+pt.pt[0], -1+pt.pt[1], 0+pt.pt[2])); clr.push_back(vec3d(1,0,1));
    vrt.push_back (vec3d(-1+pt.pt[0], 0+pt.pt[1], 0+pt.pt[2])); clr.push_back(vec3d(0,1,1));

    vind.push_back(3); vind.push_back(2); vind.push_back(0); // 3 2 0
    vind.push_back(2); vind.push_back(5); vind.push_back(0); // 2 5 0
    vind.push_back(5); vind.push_back(4); vind.push_back(0); // 5 4 0
    vind.push_back(4); vind.push_back(3); vind.push_back(0); // 4 3 0
    vind.push_back(3); vind.push_back(2); vind.push_back(1); // 3 2 1
    vind.push_back(2); vind.push_back(5); vind.push_back(1); // 2 5 1
    vind.push_back(5); vind.push_back(4); vind.push_back(1); // 5 4 1
    vind.push_back(4); vind.push_back(3); vind.push_back(1); // 4 3 1
    
    for (int i = 0; i < 24; i+=3)
    {
        fout<<"v "<<vrt[vind[i]][0]<<" "<<vrt[vind[i]][1]<<" "<<vrt[vind[i]][2]<<"\n";
        fout<<"v "<<vrt[vind[i+1]][0]<<" "<<vrt[vind[i+1]][1]<<" "<<vrt[vind[i+1]][2]<<"\n";
        fout<<"v "<<vrt[vind[i+2]][0]<<" "<<vrt[vind[i+2]][1]<<" "<<vrt[vind[i+2]][2]<<"\n";
    }
    
    for (int i = 0; i < 24; i+=3)
    {
        fout<<"c "<<clr[vind[i]][0]<<" "<<clr[vind[i]][1]<<" "<<clr[vind[i]][2]<<"\n";
        fout<<"c "<<clr[vind[i+1]][0]<<" "<<clr[vind[i+1]][1]<<" "<<clr[vind[i+1]][2]<<"\n";
        fout<<"c "<<clr[vind[i+2]][0]<<" "<<clr[vind[i+2]][1]<<" "<<clr[vind[i+2]][2]<<"\n";            
    }        


    return true;
	return true;
}