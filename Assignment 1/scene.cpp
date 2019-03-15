#include "scene.hpp"

Scene::Scene() {}

Scene::Scene(std::vector<Object*>& o, std::vector<LightSrc>& l, Plane& bk, Camera& c, mat3d& mat, Point& origin) : objs(o), lt_srcs(l), bk_plane(bk), cam(c), vcs_mat(mat), vcs_origin(origin), cam_wcs(mat * c.pt.pt + origin.pt) {
    objs.push_back(&bk_plane);
}

Scene::Scene(std::string filename){
    generateScene(filename);
}

void Scene::defineCamera(json& j_cam){
    vec3d n(j_cam["look_at"][0],j_cam["look_at"][1],j_cam["look_at"][2]);
    n.normalize();
    cam = Camera(Point((int)j_cam["offset"] * n), n);
}

void Scene::defineVCS(json& j_vcs){
    vcs_origin = Point(j_vcs["origin"][0],j_vcs["origin"][1],j_vcs["origin"][2]);
    vec3d up(j_vcs["up"][0],j_vcs["up"][1],j_vcs["up"][2]);
    vec3d v = up - up.dot(cam.look_at) * cam.look_at;
    v.normalize();
    vec3d u = cam.look_at.cross(v);
    vcs_mat << u[0], u[1], u[2], v[0], v[1], v[2], cam.look_at[0], cam.look_at[1], cam.look_at[2];
}

void Scene::defineBackPlane(json& j_bk){
    Color bg_color(j_bk["color"][0],j_bk["color"][1],j_bk["color"][2]);
    bk_plane = Plane(Point(vcs_origin.pt + j_bk["offset"] * cam.look_at), cam.look_at, bg_color);
}

void Scene::placeLightSources(json& j_lts){
    int n_lts = j_lts["size"];
    for(int i=0;i<n_lts;++i){
        json jcurr = j_lts[std::string(1,i+'0')];
        if(jcurr.empty()) throw std::runtime_error("There are still some light sources to be specified");
        
        Color rgb(jcurr["color"][0],jcurr["color"][1],jcurr["color"][2]);
        Point loc(jcurr["loc"][0],jcurr["loc"][1],jcurr["loc"][2]);
        lt_srcs.push_back(LightSrc(rgb,loc));
    }
}

void Scene::placeObjects(json& j_obj){
    int n_objs = j_obj["size"];
    for(int i=0;i<n_objs;++i){
        json jcurr = j_obj[std::string(1,i+'0')];
        if(jcurr.empty()) throw std::runtime_error("There are still some objects to be specified");
        
        if(jcurr["name"] == "plane" || jcurr["name"] == "Plane"){
            json j_pt = jcurr["pt"];
            json j_normal = jcurr["normal"];
            objs.push_back(new Plane(Point(j_pt[0],j_pt[1],j_pt[2]), vec3d(j_normal[0],j_normal[1],j_normal[2])));
        }
        else if(jcurr["name"] == "sphere" || jcurr["name"] == "Sphere"){
            json j_pt = jcurr["pt"];
            objs.push_back(new Sphere(Point(j_pt[0],j_pt[1],j_pt[2]),jcurr["radius"]));
        }
        else if(jcurr["name"] == "quadric" || jcurr["name"] == "Quadric"){
            Eigen::Matrix4d mat;
            json j_mat = jcurr["mat"];
            mat << j_mat[0], j_mat[1], j_mat[2], j_mat[3], j_mat[1], j_mat[4], j_mat[5], j_mat[6], j_mat[2], j_mat[5], j_mat[7], j_mat[8], j_mat[3], j_mat[6], j_mat[8], j_mat[9];
            objs.push_back(new Quadric(mat));
        }
        else if(jcurr["name"] == "polygon" || jcurr["name"] == "Polygon"){
            int size = jcurr["sides"];
            std::vector<Point> pts;
            for(int k=0;k<size;++k){
                json j_side = jcurr[std::string(1,k+'0')];
                if(jcurr.empty()) throw std::runtime_error("There are still some sides to be specified");
                pts.push_back(Point(j_side[0],j_side[1],j_side[2]));
            }
            objs.push_back(new Polygon(pts));
        }
        else if(jcurr["name"] == "box" || jcurr["name"] == "Box"){
            Point a(jcurr["a"][0],jcurr["a"][1],jcurr["a"][2]);
            Point b(jcurr["b"][0],jcurr["b"][1],jcurr["b"][2]);
            Point d(jcurr["d"][0],jcurr["d"][1],jcurr["d"][2]);
            Point e(jcurr["e"][0],jcurr["e"][1],jcurr["e"][2]);
            objs.push_back(new Box(a,b,d,e));
        }
        else if(jcurr["name"] == "mesh" || jcurr["name"] == "Mesh"){
        	objs.push_back(new Mesh(jcurr["file"]));
        }
        else{
            throw std::runtime_error("Unsupported object " + (std::string)jcurr["name"]);
        }

        if(!jcurr["reflectivity"].empty())  objs.back()->reflectivity = jcurr["reflectivity"];
        if(!jcurr["transparency"].empty())  objs.back()->transparency = jcurr["transparency"];
        if(!jcurr["mu"].empty())            objs.back()->mu = jcurr["mu"];
        if(!jcurr["s_exp"].empty())         objs.back()->s_exp = jcurr["s_exp"];
        
        if(!jcurr["kd"].empty()) objs.back()->kd = vec3d(jcurr["kd"][0],jcurr["kd"][1],jcurr["kd"][2]);
        if(!jcurr["ks"].empty()) objs.back()->ks = vec3d(jcurr["ks"][0],jcurr["ks"][1],jcurr["ks"][2]);
        if(!jcurr["ka"].empty()) objs.back()->ka = vec3d(jcurr["ka"][0],jcurr["ka"][1],jcurr["ka"][2]);

        // :: Define object transformation properties
        // Design decision - do not activate transform until we have at least a matrix
        if(!jcurr["d0"].empty()) objs.back()->d0 = vec3d(jcurr["d0"][0], jcurr["d0"][1], jcurr["d0"][2]);
        if(!jcurr["M"].empty()){            
            json j_mat = jcurr["M"];
            objs.back()->M << j_mat[0], j_mat[1], j_mat[2], 
                              j_mat[3], j_mat[4], j_mat[5],
                              j_mat[6], j_mat[7], j_mat[8];
            objs.back()->Minv = objs.back()->M.inverse();
            objs.back()->MT = objs.back()->M.transpose();
            objs.back()->MTinv = objs.back()->Minv.transpose();
            // std::cout<<"M\n";
            // std::cout<<objs.back()->M;
            // std::cout<<"Minv\n";
            // std::cout<<objs.back()->Minv;
            // std::cout<<"MT\n";
            // std::cout<<objs.back()->MT;
            objs.back()->transform = true;
        }
        if(!jcurr["texture"].empty()){
            json jtxt = jcurr["texture"];
            if(jtxt["type"] == "spherical" || jtxt["type"] == "Spherical"){
                std::string fname = jtxt["file"];
                std::vector<Eigen::MatrixXd> img;
                readPPM(fname,img);
                Point center(jtxt["center"][0],jtxt["center"][1],jtxt["center"][2]);
                vec3d x_axis(jtxt["x"][0],jtxt["x"][1],jtxt["x"][2]);
                vec3d z_axis(jtxt["z"][0],jtxt["z"][1],jtxt["z"][2]);
                objs.back()->hasTexture = true;
                objs.back()->txt = new SphericalTexture(img,center,x_axis,z_axis);
            }
            else if(jtxt["type"] == "planar" || jtxt["type"] == "Planar"){
                std::string fname = jtxt["file"];
                std::vector<Eigen::MatrixXd> img;
                readPPM(fname,img);
                vec3d normal(jtxt["z"][0],jtxt["z"][1],jtxt["z"][2]);
                vec3d x_axis(jtxt["x"][0],jtxt["x"][1],jtxt["x"][2]);
                objs.back()->hasTexture = true;
                objs.back()->txt = new PlanarTexture(img,normal,x_axis);
            }
            else if(jtxt["type"] == "cylindrical" || jtxt["type"] == "Cylindrical"){
                std::string fname = jtxt["file"];
                std::vector<Eigen::MatrixXd> img;
                readPPM(fname,img);
                Point center(jtxt["center"][0],jtxt["center"][1],jtxt["center"][2]);
                vec3d x_axis(jtxt["x"][0],jtxt["x"][1],jtxt["x"][2]);
                vec3d z_axis(jtxt["z"][0],jtxt["z"][1],jtxt["z"][2]);
                objs.back()->hasTexture = true;
                objs.back()->txt = new CylindricalTexture(img,center,x_axis,z_axis);
            }
            else{
                throw std::runtime_error("Unknown texture type");
            }
        }
    }
}

void Scene::generateScene(std::string filename){
    std::ifstream fin(filename);
    json j;
    fin >> j;
    if(!j["ambience"].empty()) global_Ia = vec3d(j["ambience"][0],j["ambience"][1],j["ambience"][2]);
    defineCamera(j["camera"]);
    defineVCS(j["vcs"]);
    defineBackPlane(j["background"]);
    placeLightSources(j["lights"]);
    placeObjects(j["objects"]);
    cam_wcs = Point(vcs_mat * cam.pt.pt + vcs_origin.pt);
    objs.push_back(&bk_plane);
}