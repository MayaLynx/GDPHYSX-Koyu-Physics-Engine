#pragma once
#include "Camera.h"

class OrthoCamera : public Camera
{
private:
	float near;
	float far;

public:
	OrthoCamera(float x, float y, float z, float windowWidth, float windowHeight, float near, float far);
};

