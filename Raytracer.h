#include "Light.h"

class RayTracer
{
public:
	AggregatePrimitive *primitives;
	double *RTthit;
	Intersection *in;
	vector <Light*> lights;
	int maxDepth;

	//RayTracer(Scene* theScene, double *thit, Intersection *in, int maxDepth);
	
	//void directIllumination(Ray& ray, Color* color, BRDF brdf){}
	//void pathTrace2(Ray& ray, int depth, Color* color, bool indirect);
	//void pathTrace(Ray& ray, int depth, Color* color);
	void trace(Ray& ray, int depth, Color* color){
		if (depth > maxDepth) {
			color = new Color(0, 0, 0);
			return;
		}
		float thit;
		Intersection* in;
		if (!primitives->intersect(ray, &thit, in)) {
			// No intersection
			color = new Color(0, 0, 0);
			return;
		}
		// Obtain the brdf at intersection point
		BRDF* brdf;
		LocalGeo* local = in->localGeo;
		in->primitive->getBRDF(*local, brdf);

		// There is an intersection, loop through all light source

		Ray lray;
		Color lcolor = Color (0,0,0);
		color = (*color) + (*brdf->ka);
		for (int i = 0; i < lights.size; i++) {
			lights[i]->generateLightRay(*local, &lray, &lcolor);

			// Check if the light is blocked or not
			if (!primitives->intersectP(lray))
				// If not, do shading calculation for this
				// light source
				color = (*color) + shading(*local, *brdf, lray, lcolor);
		}
		Ray* reflectRay;
		// Handle mirror reflection
		if (brdf->kr > 0) {
			reflectRay = createReflectedRay(local, ray);
			Color* tempColor = new Color(0, 0, 0);
			// Make a recursive call to trace the reflected ray
			trace(*reflectRay, depth + 1, tempColor);
			Color* addColor = ((*brdf->kr) * (*tempColor));
			color = (*color) + (*addColor);
		}

	}
	Color shading(LocalGeo local, BRDF brdf, Ray lray, Color lcolor){
		Color result = Color(0.0, 0.0, 0.0);

		Vector *norm = new Vector (local.normal->x,local.normal->y,local.normal->z);
		double ldirDotN = (*lray.dir).dot(*norm);
		if (ldirDotN < 0) ldirDotN = 0;

		Color kdTerm;
		kdTerm.setEqual(brdf.myKd * ldirDotN);
		Color ksTerm;
		ksTerm.setEqual(brdf.myKs * nDotHAngle_toShininess);
		Color kdTermPlusKsTerm;
		kdTermPlusKsTerm.setEqual(kdTerm + ksTerm);
		Color theRest;
		Color LightColor;
		LightColor.setEqual(lcolor);
		LightColor /= (totalAttenuation);
		theRest.setEqual(LightColor * kdTermPlusKsTerm);
		result += theRest;
		return result;
	}
	Ray createReflectedRay(LocalGeo* local, Ray &ray)();
	//Ray createRefractedRay(LocalGeo local, Ray &ray, float rindex, bool* refract);
};
