#include "Objects.h"

class Sampler
{
public:

		int sceneWidth, sceneHeight;
		float widthInterval, heightInterval;
		Vector *UL, *UR, *LL, *LR;
		float xStartCoord, yStartCoord;
		Sampler(int width, int height, Vector* UL_corner, Vector* UR_corner, Vector* LL_corner, Vector* LR_corner) {
			sceneWidth = width; sceneHeight = height;
			UL = UL_corner; UR = UR_corner; LL = LL_corner; LR = LR_corner;
			widthInterval = (UR->x - UL->x) / width; heightInterval = (UL->y - LL->y) / height;
			xStartCoord = LL->x + widthInterval / 2; yStartCoord = LL->y + heightInterval / 2;
		}

		Sample* nextSample(int xPixel, int yPixel) {
			float x = xStartCoord + xPixel*widthInterval;
			float y = yStartCoord + yPixel*heightInterval;

			if (x >= UR->x || y >= UR->y)
				return new Sample(-1, -1);

			return new Sample(x, y);
		}
};
