#include "Raytracer.h"
#include "png++-0.2.7/png.hpp"

class Film
{

public:
	int width, height;
	Color**colorArray;
	Film(int w, int h) {
		width = w;
		height = h;
		colorArray = new Color*[w];
		for (int i = 0; i < width; i++) {
			colorArray[i] = new Color[h];
			for (int j = 0; j < height; j++){
				colorArray[i][j] = Color(0.0, 0.0, 0.0);
			}
		}
	}
	void commit(Sample sample, Color color){
		colorArray[sample->x][sample->y] = color;
	}

	void writeImage();
	std::ofstream file("output.png", std::ios::binary);
	png::image< png::rgb_pixel > image(width, height);
	for (size_t y = 0; y < image.get_height(); ++y)
	{
		for (size_t x = 0; x < image.get_width(); ++x)
		{
			float r = *colorArray[x][y]->r;
			float g = *colorArray[x][y]->g;
			float b = *colorArray[x][y]->b;
			Color* color = new Color(r, g, b)
				image[y][x] = png::rgb_pixel(r, g, b);
			// non-checking equivalent of image.set_pixel(x, y, ...);
		}
	}
	image.writeImage("output.png");
};