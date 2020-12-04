#pragma once
#include "Vector4d.h"
#include <vector>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

using namespace std;

class util
{
private:
	static void RGBtoHSV(vector<float>& rgb, float hsv[3]);
	static void HSVtoRGB(float H, float S, float V, float rgb[3]);
public:
	static bool equalFloats(float a, float b);
	static float cleanFloat(float value);
	static void changeSB(Vector4d rbg, Vector4d* newRgb, float sat, float bright);
};

