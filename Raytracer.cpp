
#include "Shape.h"

int main(){
	Point* v = new Point(1, 2, 3);
	
	Point* b = new Point(3, 4, 5);
	cout << v->x << endl;
	Vector *line = (*v) - *b;
	Normal *nmm = new Normal(line);
	(*nmm).print();

	LocalGeo *lg = new LocalGeo();
	

	Ray* ray = new Ray(new Point(0, 0, 0), new Vector(0, 0, 1));
	double t_hit;
	Sphere* sp2 = new Sphere(0, 0, 4, 1);

	
	Point *pt1 = new Point(0, 0, 1);
	Point *pt2 = new Point(-1, 1,3);
	Point *pt3 = new Point(1, 2, 2);
	Triangle* tri = new Triangle(pt1, pt2, pt3);
	cout << "checking for triangle intersection:" << (*tri).intersect(*ray,&t_hit,lg) << endl;
	cout << "t_hit" << t_hit << endl;
	cout << "normal:";
	(*lg).print();
	system("pause");
	return 0;
}
