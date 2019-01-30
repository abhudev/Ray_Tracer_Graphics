#include "light.hpp"
#include <stdio.h>

LightSrc::LightSrc(const Color& c, const Point& p) : il(c), loc(p) {}

bool get_refracted(const Eigen::Vector3d& N, const Eigen::Vector3d& I, const double& mu, Eigen::Vector3d& T){
    // Check for Normalization
    double cos1 = N.dot(-1.0*I);
    double sin1 = sqrt(1 - cos1*cos1);
    if(sin1 > mu && mu < 1) return false;        // :: Check total internal reflection ::

    double sin2 = (1.0/mu) * sin1;    
    double cos2 = sqrt(1 - sin2*sin2);
    T = (1.0/mu) * (I + cos1*N) - cos2*N;
    T.normalize();
    return true;
}

void light_src_interaction(Point& startPoint, Object* objcur, std::vector<Object*>& list_obj, std::vector<LightSrc>& light_sources, Eigen::Vector3d& cast_rd, Plane& bk_plane, Color& clr){
    if(objcur == &bk_plane){
        clr = bk_plane.get_color();                 // Backplane is part of Object List
        return;
    }
    Color total_diffuse_val(0,0,0), total_specular_val(0,0,0);
    // printf("Start point: [%f, %f, %f]\n", startPoint.pt[0], startPoint.pt[1], startPoint.pt[2]);
    for(auto lt_src : light_sources){
        Ray shadow_ray(startPoint, lt_src.loc - startPoint);
        shadow_ray.rd.normalize();
        Ray shadow_ray_bias(Point(startPoint.pt + bias*shadow_ray.rd), shadow_ray.rd);
        double t_src = shadow_ray_bias.get_t(lt_src.loc);
        // printf("T_SRC: %f, LIGHT_SRC: [%f, %f, %f]\n", t_src, lt_src.loc.pt[0], lt_src.loc.pt[1], lt_src.loc.pt[2]);
        // printf("Shadow ray: r0: [%f, %f, %f], rd: [%f, %f, %f]\n", shadow_ray_bias.ro.pt[0], shadow_ray_bias.ro.pt[1], shadow_ray_bias.ro.pt[2], shadow_ray_bias.rd[0], shadow_ray_bias.rd[1], shadow_ray_bias.rd[2]);
        bool shadow = false;

        // :: Check shadow object ::
        for(auto obj : list_obj){
            double t;
            obj->intersect(shadow_ray_bias, t);
            // TODO - epsilon
            if(t > 1e-6 && t < t_src){
                // printf("T_SHADOW: %f\n", t);
                shadow = true;
                break;
            }

            if(!shadow){
                // printf("NO SHADOW!!\n");
                Ray obj_normal;
                // printf("Point: [%f, %f, %f]\n", startPoint.pt[0], startPoint.pt[1], startPoint.pt[2]);
                objcur->get_normal(startPoint,obj_normal);
                Eigen::Vector3d normal_rd(obj_normal.rd), reflected_rd;
                shadow_ray.rd.normalize();
                get_reflected(normal_rd, shadow_ray.rd, reflected_rd);
                // Should we normalize reflected_rd or not?
                reflected_rd.normalize();
                // printf("Normal: (%f, %f, %f)\n", normal_rd[0], normal_rd[1], normal_rd[2]);
                // printf("Shadow: (%f, %f, %f)\n", shadow_ray.rd[0], shadow_ray.rd[1], shadow_ray.rd[2]);
                double diffuse_cos = get_diffuse(normal_rd, shadow_ray.rd);
                Color diffuse_val(lt_src.il * diffuse_cos);
                Eigen::Vector3d inv_cast_rd = -1.0*cast_rd;
                // printf("Reflected: (%f, %f, %f)\n", reflected_rd[0], reflected_rd[1], reflected_rd[2]);
                // printf("Inv_cast: (%f, %f, %f)\n", inv_cast_rd[0], inv_cast_rd[1], inv_cast_rd[2]);
                double specular_cos = get_specular(reflected_rd, inv_cast_rd, objcur->s_exp);
                if(diffuse_cos <= 0) specular_cos = 0;
                Color specular_val(lt_src.il * specular_cos);
                // Color specular_val;

                total_diffuse_val = total_diffuse_val + diffuse_val;
                total_specular_val = total_specular_val + specular_val;
            }
        }
    }

    total_diffuse_val = Color(total_diffuse_val.rgb.cwiseProduct(objcur->kd));
    total_specular_val = Color(total_specular_val.rgb.cwiseProduct(objcur->ks));    
    Color ambient_val = Color(objcur->ka.cwiseProduct(global_Ia));
    // printf("Diffuse val: [%f, %f, %f] with kd = [%f, %f, %f]\n", total_diffuse_val.rgb[0], total_diffuse_val.rgb[1], total_diffuse_val.rgb[2], objcur->kd[0], objcur->kd[1], objcur->kd[2]);
    // printf("Specular val: [%f, %f, %f] with ks = [%f, %f, %f]\n", total_specular_val.rgb[0], total_specular_val.rgb[1], total_specular_val.rgb[2], objcur->ks[0], objcur->ks[1], objcur->ks[2]);
    // printf("Ambient val: [%f, %f, %f] with ka = [%f, %f, %f]\n", ambient_val.rgb[0], ambient_val.rgb[1], ambient_val.rgb[2], objcur->ka[0], objcur->ka[1], objcur->ka[2]);

    clr = total_diffuse_val + total_specular_val + ambient_val;
    // clr = total_specular_val + ambient_val;
    // clr = total_diffuse_val +  ambient_val;
}