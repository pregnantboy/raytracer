#include "Light.h"

class Raytracer
{
public:
	AggregatePrimitive *primitives;
	//double *RTthit;
	
	vector <Light*> lights;
	int maxDepth;

	//RayTracer(Scene* theScene, double *thit, Intersection *in, int maxDepth);
	
	//void directIllumination(Ray& ray, Color* color, BRDF brdf){}
	//void pathTrace2(Ray& ray, int depth, Color* color, bool indirect);
	//void pathTrace(Ray& ray, int depth, Color* color);
	void trace(Ray& ray, int depth, Color& color){
		Intersection *in = new Intersection();
		if (depth > maxDepth) {
			//reach max depth
			color = Color(0, 0, 0);
			return;
		}
		float thit;

		if (!primitives->intersect(ray, &thit, in)) {
			color = Color(0, 0, 0);
			return;
		}
		
		//cout << "hit"<< endl;
		// Obtain the brdf at intersection point
		BRDF* brdf = new BRDF();
		LocalGeo* local = in->localGeo;
		in->primitive->getBRDF(*local, *brdf);
		
		// There is an intersection, loop through all light source

		Ray lray;
		Color lcolor = Color (0,0,0);
		//color = (*color) + (*brdf->ka);
		for (vector<Light*>::iterator l = lights.begin(); l != lights.end(); l++) {

			(*l)->generateLightRay(*local, &lray, lcolor);

			// Check if the light is blocked or not

			if (!primitives->intersectP(lray)){
			//cout << "light is through" << endl;
			// If not, do shading calculation for this
			// light source
				Color* resultingcolor = (color) + shading(*local, *brdf, lray, lcolor, ray);
				color = *resultingcolor;
				//color.print();
			}
			
		}
		Ray* reflectRay;
		// Handle mirror reflection
		
		if (brdf->kr > 0) {
			reflectRay = createReflectedRay(local, ray);
			Color* tempColor = new Color(0, 0, 0);
			// Make a recursive call to trace the reflected ray
			trace(*reflectRay, depth + 1, *tempColor);
			Color* addColor = ((*brdf->kr) * (*tempColor));
			Color*resultingcolor = color + *addColor;
			color = *resultingcolor;
		}

	}
	Color shading(LocalGeo local, BRDF brdf, Ray lray, Color lcolor,Ray ray){
		Color *result = new Color(0.0, 0.0, 0.0);

		Vector *norm = new Vector (local.normal->x,local.normal->y,local.normal->z);
		double ldirDotN = (*lray.dir).dot(*norm);
		if (ldirDotN < 0) ldirDotN = 0;
		Vector* reflectedDir = (*norm)*(2 * ldirDotN);
		reflectedDir = (*reflectedDir) - (*lray.dir);
		reflectedDir = reflectedDir->normalize();				//i should right?
		Vector* viewingray = (*lray.dir)*(-1.0);  // negative dir of ray
		double rdotv = (*reflectedDir).dot(*viewingray);  
		if (rdotv < 0) rdotv = 0;

		Color *kdTerm;
		kdTerm = (*brdf.kd) * ldirDotN;
		Color *ksTerm;
		ksTerm = (*brdf.ks)* rdotv;
		Color *kaTerm = new Color (brdf.ka->r,brdf.ka->g,brdf.ka->b);
		
		Color *totalshading = new Color(0, 0, 0);
		totalshading = (*kdTerm) + (*ksTerm);
		totalshading = (*totalshading) + (*kaTerm);

		result = (lcolor) * (*totalshading);

		return *result;
	}
	Ray* createReflectedRay(LocalGeo* local, Ray &ray){
		Vector *norm = new Vector(local->normal->x, local->normal->y, local->normal->z);
		double dirDotN = (*ray.dir).dot(*norm);
		Vector* reflectedDir = (*norm)*(2 * dirDotN);
		reflectedDir = (*reflectedDir) - (*ray.dir);

		Point* newPos = new Point(local->pos->x + 0.1*reflectedDir->x, local->pos->y + 0.1*reflectedDir->y, local->pos->z + 0.1*reflectedDir->z);
		return new Ray(newPos, reflectedDir);

	};
	//Ray createRefractedRay(LocalGeo local, Ray &ray, float rindex, bool* refract);
};
