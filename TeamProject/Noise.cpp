#include "noise.h"
#include "perlin.h"

////////PERLIN NOISE

double PerlinNoise::noise2D(double x, double y, double alpha, double beta, int n)
{
	int i;
	double sum = 0;
	double p[2], scale = 1;

	p[0] = x;
	p[1] = y;
	for (i = 0; i < n; i++) {
		double val = perlin::noise2(p);
		sum += val / scale;
		scale *= alpha;
		p[0] *= beta;
		p[1] *= beta;
	}
	return(sum);
}