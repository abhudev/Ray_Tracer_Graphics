#include "sphere.hpp"

Sphere::Sphere(const Point& p, const float r) : Object("Sphere"), id(obj_count++), center(p), radius(r) {}

bool Sphere::internal_intersect(Ray& r, double& t, int& args){
    double rdnorm = r.rd.norm();
    vec3d normed_rd = r.rd/rdnorm;
    // r.rd.normalize();
    vec3d l = center - r.ro;
    double tca = l.dot(normed_rd);
    if(tca < 0) return false;
    
    double d = l.squaredNorm() - tca*tca;
    if(d > radius*radius) return false;
    
    double thc = sqrt(radius*radius - d);    
    t = tca < thc ? tca + thc : tca - thc;
    t /= rdnorm;
    return true;
}

bool Sphere::internal_get_normal(Point& p, Ray& r, int& args){
    if(abs((p-center).norm() - radius) > eps){
        if(debug){
            printf("p: %s\n",p.toString().c_str());
            print();
            throw std::runtime_error("Point not on sphere");
        }
        return false;
    }
    r = Ray(p,(p-center).normalized());
    return true;
}

void Sphere::print(){
    printf("ID - %d | Type - Sphere\n",id);
    printf("radius: %f\n",radius);
    printf("center: %s\n",center.toString().c_str());
}

void addVertex(std::vector<std::vector<double> >& v, double x, double y, double z) {
    std::vector<double> tmp_cord;
    tmp_cord.push_back(x);
    tmp_cord.push_back(y);
    tmp_cord.push_back(z);
    v.push_back(tmp_cord);    
}

void addIndices(std::vector<int>& v, int x, int y, int z) {
    v.push_back(x);
    v.push_back(y);
    v.push_back(z);
}

void addTexCoord(std::vector<double>& v, double s, double t) {
    v.push_back(s);
    v.push_back(t);    
}

// http://www.songho.ca/opengl/gl_sphere.html
bool Sphere::get_mesh(std::ofstream& fout) {
    double sectorCount = 72;
    double stackCount = 24;

    std::vector<std::vector<double> > vertices;
    std::vector<int> indices;


    double x, y, z, xy;                              // vertex position    

    double sectorStep = 2 * PI / sectorCount;
    double stackStep = PI / stackCount;
    double sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cos(stackAngle);             // r * cos(u)
        z = radius * sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sin(sectorAngle);             // r * cos(u) * sin(v)
            addVertex(vertices, x, y, z);            
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
                addIndices(indices, k1, k2, k1+1);   // k1---k2---k1+1

            if(i != (stackCount-1))
                addIndices(indices, k1+1, k2, k2+1); // k1+1---k2---k2+1
        }
    }

    for (uint i = 0; i < indices.size(); ++i) {
        int ind = indices[i];
        std::vector<double> vv(vertices[ind]);
        vec3d p1(vv[0]+center.pt[0], vv[1]+center.pt[1], vv[2]+center.pt[2]);
        if(transform) p1 = M * p1 + d0;
        fout<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<"\n";        
    }
    
    for (uint i = 0; i < indices.size(); ++i)
        fout<<"c "<<ka[0]<<" "<<ka[1]<<" "<<ka[2]<<"\n";


    return true;
}


// -----------------------------------------------------------------------
// bool Sphere::get_mesh(std::ofstream& fout) {
    
//     const double PI = 3.1415926f;
//     double sectorCount = 72;
//     double stackCount = 24;

//     std::vector<double> vertices;    
//     std::vector<double> texCoords;
//     std::vector<int> indices;

//     // tmp vertex definition (x,y,z,s,t)
//     struct Vertex
//     {
//         double x, y, z, s, t;
//     };
//     std::vector<Vertex> tmpVertices;

//     double sectorStep = 2 * PI / sectorCount;
//     double stackStep = PI / stackCount;
//     double sectorAngle, stackAngle;

//     // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
//     for(int i = 0; i <= stackCount; ++i)
//     {
//         stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
//         double xy = radius * cos(stackAngle);       // r * cos(u)
//         double z = radius * sin(stackAngle);        // r * sin(u)

//         // add (sectorCount+1) vertices per stack
//         // the first and last vertices have same position and normal, but different tex coords
//         for(int j = 0; j <= sectorCount; ++j)
//         {
//             sectorAngle = j * sectorStep;           // starting from 0 to 2pi

//             Vertex vertex;
//             vertex.x = xy * cos(sectorAngle);      // x = r * cos(u) * cos(v)
//             vertex.y = xy * sin(sectorAngle);      // y = r * cos(u) * sin(v)
//             vertex.z = z;                           // z = r * sin(u)
//             vertex.s = (double)j/sectorCount;        // s
//             vertex.t = (double)i/stackCount;         // t
//             tmpVertices.push_back(vertex);
//         }
//     }    

//     Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
//     std::vector<double> n;                           // 1 face normal

//     int i, j, k, vi1, vi2;
//     int index = 0;                                  // index for vertex
//     for(i = 0; i < stackCount; ++i)
//     {
//         vi1 = i * (sectorCount + 1);                // index of tmpVertices
//         vi2 = (i + 1) * (sectorCount + 1);

//         for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
//         {
//             // get 4 vertices per sector
//             //  v1--v3
//             //  |    |
//             //  v2--v4
//             v1 = tmpVertices[vi1];
//             v2 = tmpVertices[vi2];
//             v3 = tmpVertices[vi1 + 1];
//             v4 = tmpVertices[vi2 + 1];

//             // if 1st stack and last stack, store only 1 triangle per sector
//             // otherwise, store 2 triangles (quad) per sector
//             if(i == 0) // a triangle for first stack ==========================
//             {
//                 // put a triangle
//                 addVertex(vertices, v1.x, v1.y, v1.z);
//                 addVertex(vertices, v2.x, v2.y, v2.z);
//                 addVertex(vertices, v4.x, v4.y, v4.z);

//                 // put tex coords of triangle
//                 addTexCoord(texCoords, v1.s, v1.t);
//                 addTexCoord(texCoords, v2.s, v2.t);
//                 addTexCoord(texCoords, v4.s, v4.t);                

//                 // put indices of 1 triangle
//                 addIndices(indices, index, index+1, index+2);                

//                 index += 3;     // for next
//             }
//             else if(i == (stackCount-1)) // a triangle for last stack =========
//             {
//                 // put a triangle
//                 addVertex(vertices, v1.x, v1.y, v1.z);
//                 addVertex(vertices, v2.x, v2.y, v2.z);
//                 addVertex(vertices, v3.x, v3.y, v3.z);

//                 // put tex coords of triangle
//                 addTexCoord(texCoords, v1.s, v1.t);
//                 addTexCoord(texCoords, v2.s, v2.t);
//                 addTexCoord(texCoords, v3.s, v3.t);
                
//                 // put indices of 1 triangle
//                 addIndices(indices, index, index+1, index+2);                

//                 index += 3;     // for next
//             }
//             else // 2 triangles for others ====================================
//             {
//                 // put quad vertices: v1-v2-v3-v4
//                 addVertex(vertices, v1.x, v1.y, v1.z);
//                 addVertex(vertices, v2.x, v2.y, v2.z);
//                 addVertex(vertices, v3.x, v3.y, v3.z);
//                 addVertex(vertices, v4.x, v4.y, v4.z);

//                 // put tex coords of quad
//                 addTexCoord(texCoords, v1.s, v1.t);
//                 addTexCoord(texCoords, v2.s, v2.t);
//                 addTexCoord(texCoords, v3.s, v3.t);
//                 addTexCoord(texCoords, v4.s, v4.t);                

//                 // put indices of quad (2 triangles)
//                 addIndices(indices, index, index+1, index+2);
//                 addIndices(indices, index+2, index+1, index+3);                

//                 index += 4;     // for next
//             }
//         }
//     }
    
//     // Print the vertices to file
//     // for (int i = 0; i < indices.size(); ++i) {
//     //     vec3d p1()
//     // }


//     return true;
// }