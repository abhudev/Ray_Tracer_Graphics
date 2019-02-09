	#include "object.hpp"

Object::Object() {}

void Object::printProperties(){
    printf("kd = %s\n",toString(kd).c_str());
    printf("ks = %s\n",toString(ks).c_str());
    printf("ka = %s\n",toString(ka).c_str());
    printf("mu = %f | s_exp = %f | reflectivity = %f | transparency = %f\n",mu, s_exp, reflectivity, transparency);
}

bool Object::intersect(Ray& r, double& t)
{
	Ray r_tmp = r;	
	double tmp;

	if(transform){
		r_tmp.ro = Point(Minv * (r.ro.pt - d0));
		r_tmp.rd = Minv * r.rd;
	}

	// return internal_intersect(r_tmp, t);

	if(!internal_intersect(r_tmp, tmp)) return false;
	t = tmp;	
	return true;

}

bool Object::get_normal(Point& p, Ray& r)
{
	Point tmp_p(p);
	
	if(transform)
	{
		tmp_p = Point(Minv*(p.pt - d0));
	}

	if(!internal_get_normal(tmp_p, r)) 
	{		
		return false;
	}

	if(transform){
		r.rd = MTinv * r.rd;
		r.rd.normalize();  // Very important fix
		r.ro = p;
	}

	return true;

	// return internal_get_normal(p, r);
}