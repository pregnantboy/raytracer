
#include "camera.h"

int main(){

	Film* film = new Film(100, 100);
	Sphere* sphere = new Sphere(10, 10, 10, 10);

	Transformation* none = new Transformation(new Matrix());
	Material* mat = new Material(new BRDF(new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3)));
	GeometricPrimitive* geosphere = new GeometricPrimitive(none, none, sphere, mat);
	vector<Primitive*> privec;
	privec.push_back(geosphere);
	AggregatePrimitive* aggpri = new AggregatePrimitive(privec);

	Vector* lightpos = new Vector(0, 0, 0);
	Color* lightcolor = new Color(0.1, 0.2, 0.3);

	PointLight* ptlight = new PointLight(lightpos,lightcolor);

	Raytracer* raytracer = new Raytracer();
	raytracer->lights.push_back(ptlight);
	raytracer->primitives = aggpri;
	raytracer->maxDepth = 5;

	Vector* camdir = new Vector(0, 0, 1);
	Vector* camup = new Vector(0, 1, 0);
	Camera* cam = new Camera(lightpos, camdir, camup, 100, 100, 100);

	Sampler* sampler = new Sampler(100, 100);
	Sample* sample = new Sample();
	while (sampler->getSample(sample)){
		Ray* ray = new Ray();
		Color* outputcolor = new Color();
		cam->generateRay(*sample, ray);
		
		raytracer->trace(*ray, 0, outputcolor);
	}

	cout << "end of output" << endl;
	/*

	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			
			Color* color = new Color(0.3, .4, .5);
			Sample* sample = new Sample(i, j);
			film->commit(sample, color);
		}
	}
	
	film->writeImage("output.png");
	cout << "output.png written" << endl;


	*/
	system("pause");
	return 0;

}
