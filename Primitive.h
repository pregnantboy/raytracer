#include "Shape.h"
class Primitive;

class Intersection
{
public:
	LocalGeo *localGeo;
	Primitive *primitive;
};
class Primitive
{
public:
	Primitive();
	virtual bool intersect(Ray& ray, float* thit, Intersection* in)=0;
	virtual bool intersectP(Ray& ray) = 0;
	virtual void getBRDF(LocalGeo& local, BRDF* brdf)=0;
};

class GeometricPrimitive:public Primitive{
public:
	Transformation* objToWorld;
	Transformation* worldToObj;
	Shape* shape;
	Material* mat;
	
	bool intersect(Ray& ray, float* thit, Intersection* in){
		Ray *oray = (*worldToObj)* (&ray);
		LocalGeo* olocal;
		if (!shape->intersect(*oray, thit, olocal))  
			return false;
		in->primitive = this;
		in->localGeo = (*objToWorld)*(olocal);
		return true;
	}
	
};
