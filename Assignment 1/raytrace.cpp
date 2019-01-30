#include "raytrace.hpp"
#include <stdio.h>
// Find the intersection of ray with closest object
Object* shootRay(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, double& tmin){
    double t;
    int idx = 0, objmin_idx = 0;
    for(auto obj : list_obj){
        // Check Equality comparison for objects
        if(obj == startObj && obj->transparency == 0) continue;
        bool chk = obj->intersect(startRay, t);
        if(chk && t < tmin && t > 0){
            tmin = t;
            objmin_idx = idx;
            // std::cout << tmin << " " << objmin_idx << "\n";
        }
        idx++;
    }
    return list_obj[objmin_idx];
}

// Write the resulting color to 
void trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, Plane& bk_plane, Color& clr){
    double tmin = 1e7;
    Object* objmin = shootRay(startRay, startObj, list_obj, tmin);
    if(objmin == &bk_plane){
        clr = bk_plane.get_color();
        return;
    }
    Point rayPoint = startRay.get_pt(tmin);
    // printf("Camera: [%f, %f, %f]\n", startRay.ro.pt[0], startRay.ro.pt[1], startRay.ro.pt[2]);
    // printf("DIR: [%f, %f, %f]\n", startRay.rd[0], startRay.rd[1], startRay.rd[2]);
    // :: Phong illumination model ::
    light_src_interaction(rayPoint, objmin, list_obj, list_light_src, startRay.rd, bk_plane, clr);
}

void recursive_trace(Ray& startRay, Object* startObj, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, int depth, Plane& bk_plane, Color& clr){
    double tmin = 1e7;
    Object* objmin = shootRay(startRay, startObj, list_obj, tmin);
    if(objmin == &bk_plane){
        clr = bk_plane.get_color();
        return;
    }
    Point rayPoint = startRay.get_pt(tmin);
    // :: Phong illumination model ::    
    Color Ip;
    light_src_interaction(rayPoint, objmin, list_obj, list_light_src, startRay.rd, bk_plane, Ip);

    if(depth >= MAX_TRACE_DEPTH){
        clr = Ip;
        return;
    }

    Color reflected_component, refracted_component;
    
    Ray normal_ray;
    objmin->get_normal(rayPoint,normal_ray);
    Eigen::Vector3d normal_rd = normal_ray.rd;

    double refract_coeff = objmin->transparency;    
    double check_inside = normal_rd.dot(startRay.rd);

    if(check_inside > 0)
        refract_coeff = 1;

    // Reflection
    if(objmin->reflectivity > 0)
    {
        Eigen::Vector3d reflected_rd, inv_start_rd;
        inv_start_rd = -1.0*startRay.rd;
        if(check_inside > 0)
        {
            Eigen::Vector3d inv_normal_rd = -1.0*normal_rd;
            get_reflected(inv_normal_rd, inv_start_rd, reflected_rd);
        }
        else
            get_reflected(normal_rd, inv_start_rd, reflected_rd);
        
        Ray reflected_ray(rayPoint, reflected_rd);
        Color reflected_component;
        recursive_trace(reflected_ray, objmin, list_obj, list_light_src, depth+1, bk_plane, reflected_component);
    }
    if(objmin->transparency > 0){
        double mu_rel = objmin->mu;
        if(check_inside > 0){
            normal_rd = -1.0 * normal_rd;
            mu_rel = (1.0/mu_rel);
        }
        Eigen::Vector3d refracted_rd;
        int chk_TIR = get_refracted(normal_rd, startRay.rd, mu_rel, refracted_rd);
        // Check for total internal reflection!
        if(chk_TIR == 0){
            Ray refracted_ray(rayPoint, refracted_rd);
            Color refracted_component;
            recursive_trace(refracted_ray, objmin, list_obj, list_light_src, depth+1, bk_plane, refracted_component);
        }
        
    }

    clr = Ip + reflected_component * objmin->reflectivity + refracted_component * refract_coeff;
}

void cast_rays(Point& cam_vcs, Point& cam_wcs, Eigen::Matrix3d& vcs_mat, Object* cam, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, Plane& bk_plane, int X, int Y, int n, Eigen::MatrixXd& img1, Eigen::MatrixXd& img2, Eigen::MatrixXd& img3){
    for (int i = 0; i < X; ++i){
        for (int j = 0; j < Y; ++j){
            if(i == 0 && j == 0) std::cout << "Starting Raytracing\n";
            Color pixVal;            
            double x_cord = i - 0.5 * (1 - 1.0/n) - X/2;
            for (int k1 = 0; k1 < n; ++k1){
                double y_cord = j - 0.5 * (1 - 1.0/n) - Y/2;
                for (int k2 = 0; k2 < n; ++k2){                    
                    Point pixPoint(x_cord, y_cord, 0);                  // View plane
                    Eigen::Vector3d dir = pixPoint.pt - cam_vcs.pt;
                    Eigen::Vector3d transform_dir = vcs_mat * dir;
                    Ray worldRay(cam_wcs, transform_dir);
                    Color clr;                    
                    trace(worldRay, cam, list_obj, list_light_src, bk_plane, clr);
                    // recursive_trace(worldRay, cam, list_obj, list_light_src, 0, bk_plane, clr);
                    pixVal = pixVal + clr;
                    y_cord += (1.0/n);
                }                    
                x_cord += (1.0/n);
            }                        
            pixVal = pixVal * (1.0/(n*n));            
            img1(i, j) = pixVal.rgb[0];            
            img2(i, j) = pixVal.rgb[1];            
            img3(i, j) = pixVal.rgb[2];            
        }        
    }
}

void rec_cast_rays(Point& cam_vcs, Point& cam_wcs, Eigen::Matrix3d& vcs_mat, Object* cam, std::vector<Object*>& list_obj, std::vector<LightSrc>& list_light_src, Plane& bk_plane, int X, int Y, int n, Eigen::MatrixXd& img1, Eigen::MatrixXd& img2, Eigen::MatrixXd& img3){
    for (int i = 0; i < X; ++i){
        for (int j = 0; j < Y; ++j){
            if(i == 0 && j == 0) std::cout << "Starting Raytracing\n";
            Color pixVal;            
            double x_cord = i - 0.5 * (1 - 1.0/n) - X/2;
            for (int k1 = 0; k1 < n; ++k1){
                double y_cord = j - 0.5 * (1 - 1.0/n) - Y/2;
                for (int k2 = 0; k2 < n; ++k2){                    
                    Point pixPoint(x_cord, y_cord, 0);                  // View plane
                    Eigen::Vector3d dir = pixPoint.pt - cam_vcs.pt;
                    Eigen::Vector3d transform_dir = vcs_mat * dir;
                    Ray worldRay(cam_wcs, transform_dir);
                    Color clr;                    
                    recursive_trace(worldRay, cam, list_obj, list_light_src, 0, bk_plane, clr);
                    pixVal = pixVal + clr;
                    y_cord += (1.0/n);
                }                    
                x_cord += (1.0/n);
            }                        
            pixVal = pixVal * (1.0/(n*n));            
            img1(i, j) = pixVal.rgb[0];            
            img2(i, j) = pixVal.rgb[1];            
            img3(i, j) = pixVal.rgb[2];            
        }        
    }
}