#pragma once

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class PerlinNoise
{
public:
	double noise2D(double x, double y, double alpha, double beta, int n);
	double noise3D(double x, double y, double z, double alpha, double beta, int n);
};