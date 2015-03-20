#include "Film.h"

class Camera 
{
public:
        Vector* eye,* direction,* up;
        double fovx, fovy;
        int width, height;
        Camera(void);
		Camera(Vector* cameraeye, Vector* cameradirection, Vector* upVect, double fovY, int screenwidth, int screenheight){
			eye = cameraeye;
			direction = cameradirection;
			up = upVect;
			fovy = fovY;
			width = screenwidth;
			height = screenwidth;
			//fovx = 2 * atan(screenwidth / 2 / (screenheight / 2 / tan(fovy / 2 * nv_to_rad)));
			fovx = fovY;
		};
		void generateRay(Sample &sample, Ray *ray){
			Vector* input = (*eye) - *direction;
			input->normalize();

			Vector* output = (*up)*(*input);
			output->normalize();

			Vector* output2=  (*input)*(*output);

			double z = height / 2 / tan(fovy / 2 * nv_to_rad);
			double fovxTerm = 2 * atan(width / 2 / z);
			double a = tan(fovxTerm / 2)*((sample.x - ((double)width / 2.0)) / ((double)width / 2.0));
			double b = tan((fovy / 2.0)*nv_to_rad)*((((double)height / 2.0) - sample.y) / ((double)height / 2.0));
			Vector* firstcomponent = (*output)*a;
			Vector* secondcomponent = (*output2)*b;
			Vector* direction = (*firstcomponent) + (*secondcomponent);
			direction = (*direction) - (*input);
			direction->normalize();

			ray->pos = new Point(eye->x, eye->y, eye->z);
			ray->dir = direction;
			ray->t_min = 0.001;
			ray->t_max = 10000;
		}
};