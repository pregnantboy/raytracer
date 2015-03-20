#include "Sampler.h"

class Shape
{
public:
	virtual bool intersect(Ray &ray, float* t_hit, LocalGeo* local) = 0;
	virtual bool intersectP(Ray &ray) = 0;
};

class Sphere: public Shape
{
public:
	double x, y, z,r;
	Sphere(){
		x = 0;
		y = 0;
		z = 0;
		r = 1;
	}
	Sphere(double xx, double yy, double zz, double radius){
		x = xx;
		y = yy;
		z = zz;
		r = radius;
	}
	bool intersectP(Ray &ray){
		Point* center = new Point(x, y, z);
		Vector *AminusC = (*ray.pos) - *center;
		//t^2D^2 + 2tD(A-C) + ((A-C)^2- r^2) = 0
		double B = (2*(*ray.dir).dot(*AminusC));
		double Bsquared = B*B;
		double A = (*ray.dir).dot(*ray.dir);
		double AC = ((*AminusC).dot(*AminusC) - r*r)*(A);
		double dis = Bsquared - 4*AC;
		float t_hit;
		if (dis >= 0){
			double sqdis = sqrt(dis);
			double t1 = (-B + (sqdis)) / (2*A);
			double t2 = (-B - (sqdis)) / (2*A);
			if ((t1 < t2) && (t1>0)){
				t_hit = (float)t1;
				//cout << "t_hit" << t_hit;
				return true;
			}
			else if (t2 > 0){
				t_hit = (float)t2;
				//cout << "t_hit" << t_hit;

				return true;
			}

		}
		return false;

	}

	bool intersect(Ray &ray, float* t_hit, LocalGeo* local){
		Point* center = new Point(x, y, z);
		Vector *AminusC = (*ray.pos) - *center;
		//t^2D^2 + 2tD(A-C) + ((A-C)^2- r^2) = 0
		double B = (2 * (*ray.dir).dot(*AminusC));
		double Bsquared = B*B;
		double A = (*ray.dir).dot(*ray.dir);
		double AC = ((*AminusC).dot(*AminusC) - r*r)*(A);
		double dis = Bsquared - 4 * AC;

		if (dis >= 0){
			double sqdis = sqrt(dis);
			double t1 = (-B + (sqdis)) / (2 * A);
			double t2 = (-B - (sqdis)) / (2 * A);
			if ((t1 < t2) && (t1>0)){
				*t_hit = t1;
			}
			else if (t2 > 0){
				*t_hit = t2;
			}
			else{
				return false;
			}
			Vector* intersect = (*ray.pos) + *((*ray.dir) * (*t_hit));
			local->pos = new Point(intersect->x, intersect->y, intersect->z);
			Vector *normal = *(local->pos) - *center;
			local->normal = new Normal(normal);
			return true;

		}
		return false;

	}
};

class Triangle : public Shape
{
public:
	Point *pt1, *pt2, *pt3;
	Normal *normal,*oppnormal;
	Vector *v1, *v2minusv1, *v3minusv1;
	Triangle(Point* ver1, Point* ver2, Point* ver3){
		pt1 = new Point( ver1->x,ver1->y,ver1->z);
		pt2 = new Point(ver2->x, ver2->y, ver2->z);
		pt3 = new Point(ver3->x, ver3->y, ver3->z);
		v1 = new Vector(pt1->x, pt1->y, pt1->z);
		v2minusv1 = (*pt2) - (*pt1);
		v3minusv1 = (*pt3) - (*pt1);
		normal = new Normal( (*v2minusv1)*(*v3minusv1));
		oppnormal = new Normal((*v3minusv1)*(*v2minusv1));
		/*
		cout << "v1:";
		(*v1).print();
		cout << "v2:";
		(*v2minusv1).print();
		cout << "v3:";
		(*v3minusv1).print();
		*/
	}
	bool intersectP(Ray &ray){
		double xe, ye, ze,
			xa, ya, za,
			xb, yb, zb,
			xc, yc, zc,
			xd, yd, zd;

		double beta, gamma;
		float t_hit;

		double a, b, c,
			d, e, f,
			g, h, i,
			j, k, l;

		double eimhf,
			gfmdi,
			dhmeg,
			akmjb,
			jcmal,
			blmkc;

		double M;

		xe = ray.pos->x; ye = ray.pos->y; ze = ray.pos->z;
		xa = pt1->x; ya = pt1->y; za = pt1->z;
		xb = pt2->x; yb = pt2->y; zb = pt2->z;
		xc = pt3->x; yc = pt3->y; zc = pt3->z;
		xd = ray.dir->x; yd = ray.dir->y; zd = ray.dir->z;

		a = xa - xb; b = ya - yb; c = za - zb;
		d = xa - xc; e = ya - yc; f = za - zc;
		g = xd; h = yd; i = zd;
		j = xa - xe; k = ya - ye; l = za - ze;

		eimhf = e*i - h*f;
		gfmdi = g*f - d*i;
		dhmeg = d*h - e*g;
		akmjb = a*k - j*b;
		jcmal = j*c - a*l;
		blmkc = b*l - k*c;

		M = a*(eimhf)+b*(gfmdi)+c*(dhmeg);

		t_hit = -((f*akmjb) + (e*jcmal) + (d*blmkc)) / M;
		if (t_hit < ray.t_min || t_hit > ray.t_max) return false;

		gamma = ((i*akmjb) + (h*jcmal) + (g*blmkc)) / M;
		if (gamma < 0 || gamma > 1) return false;

		beta = ((j*eimhf) + (k*gfmdi) + (l*dhmeg)) / M;
		if (beta < 0 || beta >(1 - gamma)) return false;

		return true;
	}

	bool intersect(Ray &ray, float* t_hit, LocalGeo* local){
		double xe, ye, ze,
			xa, ya, za,
			xb, yb, zb,
			xc, yc, zc,
			xd, yd, zd;

		double beta, gamma;

		double a, b, c,
			d, e, f,
			g, h, i,
			j, k, l;

		double eimhf,
			gfmdi,
			dhmeg,
			akmjb,
			jcmal,
			blmkc;

		double M;

		xe = ray.pos->x; ye = ray.pos->y; ze = ray.pos->z;
		xa = pt1->x; ya = pt1->y; za = pt1->z;
		xb = pt2->x; yb = pt2->y; zb = pt2->z;
		xc = pt3->x; yc = pt3->y; zc = pt3->z;
		xd = ray.dir->x; yd = ray.dir->y; zd = ray.dir->z;

		a = xa - xb; b = ya - yb; c = za - zb;
		d = xa - xc; e = ya - yc; f = za - zc;
		g = xd; h = yd; i = zd;
		j = xa - xe; k = ya - ye; l = za - ze;

		eimhf = e*i - h*f;
		gfmdi = g*f - d*i;
		dhmeg = d*h - e*g;
		akmjb = a*k - j*b;
		jcmal = j*c - a*l;
		blmkc = b*l - k*c;

		M = a*(eimhf)+b*(gfmdi)+c*(dhmeg);

		*t_hit = -((f*akmjb) + (e*jcmal) + (d*blmkc)) / M;
		if (*t_hit < ray.t_min || *t_hit > ray.t_max) return false;

		gamma = ((i*akmjb) + (h*jcmal) + (g*blmkc)) / M;
		if (gamma < 0 || gamma > 1) return false;

		beta = ((j*eimhf) + (k*gfmdi) + (l*dhmeg)) / M;
		if (beta < 0 || beta >(1 - gamma)) return false;

		Vector* intersect = (*ray.pos) + *((*ray.dir) * (*t_hit));
		local->pos = new Point(intersect->x, intersect->y, intersect->z);
		if (zd > 0 && normal->z < 0){
			local->normal = normal;
		}
		else{
			local->normal = oppnormal;
		}
		return true;
	}

};