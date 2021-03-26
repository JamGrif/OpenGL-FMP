#include "Utility.h"


//GOLD material - ambient/diffuse/specular/shininess

float* Utility::goldAmbient()
{
	static float a[4] = { 0.2473f, 0.1995f, 0.0745f, 1.0f };
	return (float*)a;
}

float* Utility::goldDiffuse()
{
	static float a[4] = { 0.7516f, 0.6065f, 0.2265f, 1.0f };
	return (float*)a;
}

float* Utility::goldSpecular()
{
	static float a[4] = { 0.6283f, 0.5559f, 0.3661f, 1.0f };
	return (float*)a;
}

float Utility::goldShininess()
{
	return 32.2f;
}
