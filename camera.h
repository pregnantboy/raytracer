#include "Film.h"

class Camera 
{
public:
		Point* eyepos;
		//camera coordinates
		Vector* u, *v, *w;
		Vector* ul, *ur, *ll,* lr;

		Camera(Point* eyepos, Vector* ul, Vector* ur, Vector* ll, Vector* lr) {
			this->eyepos = eyepos;
			this->ul = ul;
			this->ur = ur;
			this->ll = ll;
			this->lr = lr;
		}
		void generateRay(Sample* samp, Ray& ray) {
			ray.pos = eyepos;
			double u = samp->x;
			double v = samp->y;

			/*
			Vector *pv11 = (*ll)*v;
			Vector* pv12 = (*ul)*(1 - v);
			Vector* pv13 = *pv11 + *pv12;
			Vector* pv1 = (*pv12)*u;

			Vector *pv21 = (*lr)*v;
			Vector* pv22 = (*ur)*(1 - v);
			Vector* pv23 = *pv21 + *pv22;
			Vector* pv2 = (*pv23)*(1 - u);
			Vector* pv = *pv1 + *pv2;*/
			Point* p = new Point(samp->x,samp->y,0);
			
			Vector* newdir = *p - *eyepos;
			newdir = newdir->normalize();
			ray.dir = newdir;
			ray.t_min = 0;
			ray.t_max = 10000000;
		}

	
};