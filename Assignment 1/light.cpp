#include "light.hpp"

LightSrc::LightSrc(const Color& c, const Point& p) : il(c), loc(p) {}

bool LightSrc::get_mesh(std::ofstream& fout) {
    std::vector<vec3d> vrt;
    std::vector<vec3d> clr;
    std::vector<int> vind;
    vrt.push_back (vec3d(0+loc.pt[0], 0+loc.pt[1], 1+loc.pt[2])); clr.push_back(vec3d(1,0,0));
    vrt.push_back (vec3d(0+loc.pt[0], 0+loc.pt[1], -1+loc.pt[2])); clr.push_back(vec3d(0,1,0));
    vrt.push_back (vec3d(0+loc.pt[0], 1+loc.pt[1], 0+loc.pt[2])); clr.push_back(vec3d(0,0,1));
    vrt.push_back (vec3d(1+loc.pt[0], 0+loc.pt[1], 0+loc.pt[2])); clr.push_back(vec3d(1,1,0));
    vrt.push_back (vec3d(0+loc.pt[0], -1+loc.pt[1], 0+loc.pt[2])); clr.push_back(vec3d(1,0,1));
    vrt.push_back (vec3d(-1+loc.pt[0], 0+loc.pt[1], 0+loc.pt[2])); clr.push_back(vec3d(0,1,1));

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


    return true;
}

bool get_refracted(const vec3d& N, const vec3d& I, const double& mu, vec3d& T){
    double cos1 = N.dot(-1.0*I);                // Assuming Normalized vectors
    double sin1 = sqrt(1 - cos1*cos1);
    if(sin1 > mu && mu < 1) return true;        // :: Check total internal reflection ::

    double sin2 = (1.0/mu) * sin1;    
    double cos2 = sqrt(1 - sin2*sin2);
    T = (1.0/mu) * (I + cos1*N) - cos2*N;
    T.normalize();
    return false;
}

void light_src_interaction(Point& startPoint, Object* objcur, std::vector<Object*>& list_obj, std::vector<LightSrc>& light_sources, vec3d& cast_rd, Plane& bk_plane, Color& clr, bool check_normal, Ray& normal_ray , std::vector<pvv>& ray_store){
    if(objcur == &bk_plane){
        clr = bk_plane.get_color();                 // Backplane is part of Object List
        return;
    }
    Color total_diffuse_val, total_specular_val;
    Color ambient_val;
    if(objcur->hasTexture){
        ambient_val = Color((objcur->txt->getTexture(startPoint.pt)/255).cwiseProduct(global_Ia));
    }
    else{
        ambient_val = Color(objcur->ka.cwiseProduct(global_Ia));
    }
    
    // :: Return ambient if normal is not present ::
    if(!check_normal)
    {
        clr = ambient_val;
        return;
    }
    for(auto lt_src : light_sources){
        if(raysim)
            ray_store.push_back(pvv(startPoint.pt, lt_src.loc.pt));
        Ray shadow_ray(startPoint, lt_src.loc - startPoint);
        shadow_ray.rd.normalize();
        Ray shadow_ray_bias(Point(startPoint.pt + 1e-3*shadow_ray.rd), shadow_ray.rd);
        double t_src = shadow_ray_bias.get_t(lt_src.loc);
        bool shadow = false;
        int dummy;

        // :: Check shadow object ::
        for(auto obj : list_obj){
            double t = -10;
            obj->intersect(shadow_ray_bias, t, dummy);
            if(t > eps && t < t_src){
                shadow = true;
                break;
            }
        }
        if(!shadow){
            // Ray obj_normal;
            // bool check_normal = objcur->get_normal(startPoint,obj_normal);
            
            vec3d normal_rd(normal_ray.rd), reflected_rd;
            shadow_ray.rd.normalize();
            get_reflected(normal_rd, shadow_ray.rd, reflected_rd);
            // reflected_rd.normalize();            // Normalization not needed since shadow ray is normalized

            double diffuse_cos = get_diffuse(normal_rd, shadow_ray.rd);
            Color diffuse_val(lt_src.il * diffuse_cos);
            vec3d inv_cast_rd = -1.0*cast_rd;

            double specular_cos = get_specular(reflected_rd, inv_cast_rd, objcur->s_exp);
            if(diffuse_cos <= 0) specular_cos = 0;
            Color specular_val(lt_src.il * specular_cos);

            total_diffuse_val = total_diffuse_val + diffuse_val;
            total_specular_val = total_specular_val + specular_val;
            
        }
    }

    total_diffuse_val = Color(total_diffuse_val.rgb.cwiseProduct(objcur->kd));
    total_specular_val = Color(total_specular_val.rgb.cwiseProduct(objcur->ks));        

    clr = total_diffuse_val + total_specular_val + ambient_val;    
}