#define _CRT_SECURE_NO_DEPRECATE
#include "Raytracer.h"

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
	void commit(Sample* sample, Color* color){
		int x = sample->x;
		int y = sample->y;
		colorArray[x][y] = *color;
	}

	void writeImage(char * filename){
		int w = width;
		int h = height;
		FILE *f;
		unsigned char *img = NULL;
		int filesize = 54 + 3 * width*height;  //w is your image width, h is image height, both int
		if (img)
			free(img);
		img = (unsigned char *)malloc(3 * width*height);
		memset(img, 0, sizeof(img));

		for (int i = 0; i<width; i++)
		{
			for (int j = 0; j<height; j++)
			{
				int x = i; int y = (height - 1) - j;
				int r = colorArray[i][j].r * 255;
				int g = colorArray[i][j].g * 255;
				int b = colorArray[i][j].b * 255;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;
				img[(x + y*width) * 3 + 2] = (unsigned char)(r);
				img[(x + y*width) * 3 + 1] = (unsigned char)(g);
				img[(x + y*width) * 3 + 0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
		unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
		unsigned char bmppad[3] = { 0, 0, 0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(w);
		bmpinfoheader[5] = (unsigned char)(w >> 8);
		bmpinfoheader[6] = (unsigned char)(w >> 16);
		bmpinfoheader[7] = (unsigned char)(w >> 24);
		bmpinfoheader[8] = (unsigned char)(h);
		bmpinfoheader[9] = (unsigned char)(h >> 8);
		bmpinfoheader[10] = (unsigned char)(h >> 16);
		bmpinfoheader[11] = (unsigned char)(h >> 24);

		f = fopen(filename, "wb");
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i<h; i++)
		{
			fwrite(img + (w*(h - i - 1) * 3), 3, w, f);
			fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
		}
		fclose(f);
	}
};