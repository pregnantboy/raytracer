#include "Shape.h"
#include <vector>
#include <iterator>

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
	virtual bool intersect(Ray& ray, float* thit, Intersection* in)=0;
	virtual bool intersectP(Ray& ray) = 0;
	virtual void getBRDF(LocalGeo& local, BRDF& brdf)=0;
};


class GeometricPrimitive:public Primitive{
public:
	Transformation* objToWorld;
	Transformation* worldToObj;
	Shape* shape;
	Material* mat;
	GeometricPrimitive(Transformation* objectToWorld, Transformation* worldToObject, Shape* shape1, Material* mat1){
		objToWorld = new Transformation(objectToWorld->m);
		worldToObj = new Transformation(worldToObject->m);
		shape = shape1;
		mat = new Material(mat1->constantBRDF);
	}

	bool intersect(Ray& ray, float* thit, Intersection* in){
		Ray* oray = (*worldToObj)* (&ray);
		LocalGeo* olocal = new LocalGeo();
		if (!shape->intersect(*oray, thit, olocal))  {
			return false;
		}
		in->primitive = this;
		in->localGeo = (*objToWorld)*(olocal);
		return true;
	}
	bool intersectP(Ray& ray) {
		Ray* oray = (Ray*) malloc(sizeof(Ray));
		oray = (*worldToObj)* (&ray);
		return shape->intersectP(*oray);
	}

	void getBRDF(LocalGeo& local, BRDF& brdf) {
		mat->getBRDF(local, brdf);
	}
	
};

//class to find closest intersection
class AggregatePrimitive : public Primitive{
public:
	vector <Primitive*> primitives;
	AggregatePrimitive(vector<Primitive*> list){
		primitives.reserve(list.size());
		copy(list.begin(), list.end(), std::back_inserter(primitives));
	}
	/* syntax:
	GeometricPrimitive *p1 = new GeometricPrimitive(tra, tra, tri, mat);
	GeometricPrimitive *p2 = new GeometricPrimitive(tra, tra, tri, mat2);
	GeometricPrimitive *p3 = new GeometricPrimitive(tra, tra, tri, mat3);

	vector<Primitive*> pri;
	pri.push_back( p1);
	pri.push_back(p2);
	pri.push_back(p3);

	AggregatePrimitive* prigrand = new AggregatePrimitive(pri);
	GeometricPrimitive *p4 = (GeometricPrimitive*) (prigrand->primitives[2]);
	p4->mat->constantBRDF->print();
	*/
	bool intersect(Ray& ray, float* thit, Intersection* in){ 
		double closestt = 9999999;
		Primitive* closestp = NULL;
		for (int i = 0; i < primitives.size(); i++){
			if (primitives[i]->intersect(ray,thit,in)){
				if (*thit < closestt){
					closestt = *thit;
					closestp = primitives[i];
				}
			}
		}
		if (closestt == 9999999){
			return false;
		}
		if (closestp == NULL){
			return false;
		}
		else{
			closestp->intersect(ray, thit, in);
			return true;
		}
	}
	bool intersectP(Ray& ray){ 
		for (int i = 0; i < primitives.size(); i++){
			if (primitives[i]->intersectP(ray)){
				return true;
			}
		}
		return false;
	}
	void getBRDF(LocalGeo& local, BRDF& brdf) { 
		cout << "error: getBRDF called from aggregate primitive" << endl;
		system("pause");
		// This should never get called, because in->primitive will never be an aggregate primitive
	}
};

