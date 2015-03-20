
#include "camera.h"

int main(){
	int width = 500;
	int height = 300;
	Film* film = new Film(width, height);
	Sphere* sphere = new Sphere(0, 0, 5, 2);
	Triangle *triangle = new Triangle(new Point(2, 2, 5), new Point(-2, -2, 5), new Point(-2, 2, 5));

	Transformation* none = new Transformation(new Matrix());
	Material* mat = new Material(new BRDF(new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3), new Color(0.3, 0.3, 0.3)));
	GeometricPrimitive* geosphere = new GeometricPrimitive(none, none, sphere, mat);
	
	GeometricPrimitive* geotri = new GeometricPrimitive(none, none, triangle, mat);
	
	vector<Primitive*> privec;
	//privec.push_back(geosphere);
	privec.push_back(geotri);
	AggregatePrimitive* aggpri = new AggregatePrimitive(privec);

	Vector* lightpos = new Vector(4,4, 1);
	Color* lightcolor = new Color(0.5, 0.2, 0.3);
	Vector* lightpos2 = new Vector(-4,-4, -1);
	Color* lightcolor2 = new Color(0.2, 0.2, 0.45);
	PointLight* ptlight = new PointLight(lightpos,lightcolor);
	DirectionalLight* dirLight = new DirectionalLight(lightpos2,lightcolor2);

	Raytracer* raytracer = new Raytracer();
	raytracer->lights.push_back(ptlight);
	//raytracer->lights.push_back(dirLight);
	raytracer->primitives = aggpri;
	raytracer->maxDepth = 5;

	Point* eye = new Point(0, 0 ,-10);	
	Vector* ul = new Vector(-2, 2, 0);
	Vector* ur = new Vector(2, 2, 0);
	Vector* ll = new Vector(-2,-2, 0);
	Vector* lr = new Vector(2, 2, 0);
	Camera* cam = new Camera(eye, ul,ur,ll,lr);

	Sampler* sampler = new Sampler(width,height,ul,ur,ll,lr);
	
	for (int i=0; i < width; i++){
		for (int j=0; j < height; j++){
			Sample* sample = sampler->nextSample(i, j);
			Ray* ray = new Ray();
			Color* outputcolor = new Color();
			
			cam->generateRay(sample, *ray);
			//ray = new Ray(new Point(0, 0, 0), new Vector(0, 0, 1));
			//ray->print();

			raytracer->trace(*ray, 0, *outputcolor);
			/*if (!outputcolor->b == 0){
				//outputcolor->print();
			}*/
			film->commit(new Sample(i,j), outputcolor);
		}
	}
	

	film->writeImage("output3.png");
	cout << "output.png written" << endl;
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
