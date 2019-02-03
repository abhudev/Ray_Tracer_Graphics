#include "raytrace.hpp"

// Find the intersection of ray with closest object
Object* shootRay(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, double& tmin){
    double t;
    int idx = 0, objmin_idx = 0;
    for(auto obj : list_obj){
        if(obj == startObj && obj->transparency == 0){
            idx++;
            continue;
        }
        bool chk = obj->intersect(startRay, t);
        if(chk && t < tmin && t > 0){
            tmin = t;
            objmin_idx = idx;
        }
        idx++;
    }
    return list_obj[objmin_idx];
}

void recursive_trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, int depth, Plane& bk_plane, Color& clr){
    double tmin = std::numeric_limits<double>::infinity();
    startRay.rd.normalize();
    
    Object* objmin = shootRay(startRay, startObj, list_obj, tmin);

    if(!(tmin < std::numeric_limits<double>::infinity())){
        clr = Color(0,0,0);
        return;
    }
    
    if(objmin == &bk_plane){
        clr = bk_plane.get_color();
        return;
    }
    Point rayPoint = startRay.get_pt(tmin);

    // :: Phong illumination model ::    
    Color Ip;
    light_src_interaction(rayPoint, objmin, list_obj, list_light_src, startRay.rd, bk_plane, Ip);

    if(depth >= max_trace_depth){
        clr = Ip;
        return;
    }

    Color reflected_component, refracted_component;
    
    Ray normal_ray;
    bool check_normal = objmin->get_normal(rayPoint,normal_ray);
    if(!check_normal)
    {
        clr = Ip;
        return;
    }
    vec3d normal_rd = normal_ray.rd;

    double refract_coeff = objmin->transparency;    
    double check_inside = normal_rd.dot(startRay.rd);

    if(check_inside > eps)
        refract_coeff = 1;

    // Reflection
    if(objmin->reflectivity > 0){
        vec3d reflected_rd, inv_start_rd;
        inv_start_rd = -1.0*startRay.rd;
        if(check_inside > eps){
            vec3d inv_normal_rd = -1.0*normal_rd;
            get_reflected(inv_normal_rd, inv_start_rd, reflected_rd);
        }
        else get_reflected(normal_rd, inv_start_rd, reflected_rd);
        
        Ray reflected_ray(Point(rayPoint.pt+1e-5*reflected_rd), reflected_rd);
        recursive_trace(reflected_ray, objmin, list_obj, list_light_src, depth+1, bk_plane, reflected_component);
    }
    if(objmin->transparency > 0){
        double mu_rel = objmin->mu;
        if(check_inside > eps){            
            normal_rd = -1.0 * normal_rd;
            mu_rel = (1.0/mu_rel);
        }
        vec3d refracted_rd;
        bool chk_TIR = get_refracted(normal_rd, startRay.rd, mu_rel, refracted_rd);
        // Check for total internal reflection
        if(!chk_TIR){
            Ray refracted_ray(Point(rayPoint.pt+1e-5*refracted_rd), refracted_rd);
            recursive_trace(refracted_ray, objmin, list_obj, list_light_src, depth+1, bk_plane, refracted_component);
        }
        
    }
    clr = Ip + reflected_component * objmin->reflectivity + refracted_component * refract_coeff;
}

void cast_rays(Scene& scn, int width, int height, int n, std::vector<Eigen::MatrixXd>& img){
    #pragma omp parallel for
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            if(i == 0 && j == 0) printf("Starting Raytracing\n");
            Color pixVal;
            double x_cord = i - 0.5 * (1 - 1.0/n) - width/2;
            for (int k1 = 0; k1 < n; ++k1){
                double y_cord = j - 0.5 * (1 - 1.0/n) - height/2;
                for (int k2 = 0; k2 < n; ++k2){                    
                    Point pixPoint(x_cord, y_cord, 0);                  // Point on the View plane
                    vec3d dir = pixPoint.pt - scn.cam.pt.pt;
                    vec3d transform_dir = scn.vcs_mat * dir;
                    Ray worldRay(scn.cam_wcs, transform_dir);
                    Color clr;                                                            
                    recursive_trace(worldRay, &scn.cam, scn.objs, scn.lt_srcs, 0, scn.bk_plane, clr);
                    pixVal = pixVal + clr;
                    y_cord += (1.0/n);
                }                    
                x_cord += (1.0/n);
            }                        
            pixVal = pixVal * (1.0/(n*n));            
            for(int idx=0;idx<3;++idx) img[idx](i, j) = pixVal.rgb[idx];
        }        
    }
}