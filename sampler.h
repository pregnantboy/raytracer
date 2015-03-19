#include "Objects.h"

class Sampler
{
public:
        int sampW, sampH, numPix, index;
		Sampler(int width, int height){
			sampW = width;
			sampH = height;
			numPix = sampW * sampH;
			index = 0;
		}
		bool getSample(Sample *s){
			if (index < numPix) {
				s->x = (float)(index % sampW);
				s->y = (float)(index / sampW);
				index++;
				return true;
			}
			else {
				return false;
			}
		}
};
