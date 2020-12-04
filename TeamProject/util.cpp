#include "util.h"

#include <iostream>
#include <math.h> 
#include <algorithm>
#include <vector>

using namespace std;

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

const float EPSILON = 0.0001f;

bool util::equalFloats(float a, float b) 
{
	float diff = a - b;
	if (diff < 0) return diff *= -1;

	return diff < EPSILON;
}

float util::cleanFloat(float value) {

	float newValue;
	if (value > 0) {
		newValue = floor(value);
		if (value - newValue < EPSILON) {
			return newValue;
		}

	}
	else if (value < 0) {

		newValue = ceil(value);
		if (abs(value - newValue) < EPSILON) {
			return newValue;
		}
	}
	return value;
}

void util::RGBtoHSV(vector<float>& rgb, float hsv[3]) {

	float hue = 0.0f;
	float sat;


	float maxval, minval;

	maxval = *max_element(rgb.begin(), rgb.end());

	minval = *min_element(rgb.begin(), rgb.end());

	float difference = maxval - minval;

	float red, green, blue;
	red = rgb.at(0);
	green = rgb.at(1);
	blue = rgb.at(2);

	if (difference == 0)
		hue = 0;
	else if (red == maxval)
		hue = (float) fmod(((60 * ((green - blue) / difference)) + 360), 360.0);
	else if (green == maxval)
		hue = (float) fmod(((60 * ((blue - red) / difference)) + 120), 360.0);
	else if (blue == maxval)
		hue = (float) fmod(((60 * ((red - green) / difference)) + 240), 360.0);

	hsv[0] = hue;


	if (maxval == 0)
		sat = 0;
	else
		sat = 100 * (difference / maxval);

	hsv[1] = (sat);

	hsv[2] = (maxval * 100);
}

void util::HSVtoRGB(float H, float S, float V, float rgb[3]) {
	if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		cout << "The givem HSV values are not in valid range" << endl;
		return;
	}
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs((float) fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	float R = r + m;
	float G = g + m;
	float B = b + m;
	rgb[0] = R;
	rgb[1] = G;
	rgb[2] = B;
}

void util::changeSB(Vector4d rbg, Vector4d* newRgb, float sat, float bright) {
	float values[] = {
		rbg.getX(),
		rbg.getY(),
		rbg.getZ()
	};
	vector<float> vecRgb(values, values + sizeof(values) / sizeof(float));

	//TODO Sanitize values (e.g: If S - sat < 0, then S  = 0)
	float hsv[3];
	RGBtoHSV(vecRgb, hsv);
	hsv[1] += sat;
	hsv[2] += bright;

	float rgb[3];
	HSVtoRGB(hsv[0], hsv[1], hsv[2], rgb);
	*newRgb = Vector4d(rgb[0], rgb[1], rgb[2], 1);
}