#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera
{
private:
	float fieldOfView;

public:
	PerspectiveCamera(float x, float y, float z, float fov, float windowWidth, float windowHeight);
	glm::mat4 getProjectionMatrix();
};