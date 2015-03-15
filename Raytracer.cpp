
#include "Primitive.h"

int main(){
	Point* v = new Point(1, 2, 3);
	
	Point* b = new Point(3, 4, 5);
	//cout << v->x << endl;
	Vector *line = (*v) - *b;
	Normal *nmm = new Normal(line);
	

	LocalGeo *lg = new LocalGeo();
	

	Ray* ray = new Ray(new Point(0, 0, 0), new Vector(0, 0, 1));
	float t_hit;
	Sphere* sp2 = new Sphere(0, 0, 4, 1);

	
	Point *pt1 = new Point(0, 0, 1);
	Point *pt2 = new Point(-1, 1,3);
	Point *pt3 = new Point(1, 2, 2);
	Triangle* tri = new Triangle(pt1, pt2, pt3);
	cout << "checking for triangle intersection:" << (*tri).intersect(*ray,&t_hit,lg) << endl;
	cout << "t_hit" << t_hit << endl;
	cout << "normal:";
	(*lg).print();
	Color* color = new Color(1, 1, 1);
	Color* color2 = new Color(2, 1, 1);
	Color* color3 = new Color(3, 1, 1);
	Shape* shape = tri;
	Material *mat = new Material(new BRDF(color, color,color, color));
	Material *mat2 = new Material(new BRDF(color2, color2, color2, color2));
	Material *mat3 = new Material(new BRDF(color3, color3, color3, color3));
	Matrix* m = new Matrix(1, 3, 2, 1);
	Transformation* tra = new Transformation(m);
	tra->m->print();
	tra->minvt->print();
	/*
	lg = (*tra)*(lg);
	lg->normal->print();
	*/

	Primitive *p1 = new GeometricPrimitive(tra, tra, tri, mat);
	GeometricPrimitive *p2 = new GeometricPrimitive(tra, tra, tri, mat2);
	GeometricPrimitive *p3 = new GeometricPrimitive(tra, tra, tri, mat3);

	vector<Primitive*> pri;
	pri.push_back( p1);
	pri.push_back(p2);
	pri.push_back(p3);

	AggregatePrimitive* prigrand = new AggregatePrimitive(pri);
	GeometricPrimitive *p4 = (GeometricPrimitive*) (prigrand->primitives[2]);
	p4->mat->constantBRDF->print();



	system("pause");
	return 0;
}
