#pragma once

class PerlinNoise
{
public:
	double noise2D(double x, double y, double alpha, double beta, int n);
	double noise3D(double x, double y, double z, double alpha, double beta, int n);
};