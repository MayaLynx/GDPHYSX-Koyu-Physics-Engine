#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float x, float y, float z, float windowWidth, float windowHeight, float near, float far)
	: Camera(x, y, z)
	, near(near)
	, far(far)
{
	projection = glm::ortho(
		-windowWidth,
		windowWidth,
		-windowHeight,
		windowHeight,
		near,
		far
	);
}

glm::mat4 OrthoCamera::getProjectionMatrix()
{
	return projection;
}