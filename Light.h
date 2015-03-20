#include "Primitive.h"

class Light {
public:
	Light(){}
	virtual void generateLightRay(LocalGeo &local, Ray *lray, Color &lcolor) = 0;
	//virtual void getAtt(double* att) = 0;
};
class PointLight : public Light {
public:
	Vector *pos;
	Color *color;
	PointLight() { };
	PointLight(Vector* p, Color* c) {
	pos = new Vector (p->x,p->y,p->z);
	color = new Color (c->r,c->g,c->b);
  }
  
void generateLightRay(LocalGeo &local, Ray *lray, Color &lcolor) {
        lray->dir = ((*pos) - Vector(local.pos->x, local.pos->y, local.pos->z) );
        lray->dir = lray->dir->normalize();
		lcolor = Color(color->r, color->g, color->b);
		lray->pos = new Point(local.pos->x + .001*lray->dir->x, local.pos->y + .001*lray->dir->y, local.pos->z + .001*lray->dir->z);
        lray->t_min = 0;
        lray->t_max = 99999;
}

/*
void getAtt(double* att) {
        *att = attenuation[0];
        *(att+1) = attenuation[1];
        *(att+2) = attenuation[2]
      }

*/
};

class DirectionalLight : public Light {
public:
	Vector* dir;
	Color* color;
	DirectionalLight (Vector* d, Color* c) {
		dir = new Vector(d->x, d->y, d->z);
		dir = dir->normalize();
		color = new Color(c->r, c->g, c->b);
	 }

  void generateLightRay(LocalGeo &local, Ray *lray, Color &lcolor)
  {
        lray->pos =  new Point(local.pos->x+.001*dir->x, local.pos->y+.001*dir->y, local.pos->z+.001*dir->z);
        lray->dir = dir->normalize();
		lcolor = Color(color->r, color->g, color->b);
        lray->t_min = 0;
        lray->t_max = 999999;
  }
  /*
  void getAtt(double* att) {
        *att = 1;
        *(att+1) = 0;
        *(att+2) = 0;
  }*/
};