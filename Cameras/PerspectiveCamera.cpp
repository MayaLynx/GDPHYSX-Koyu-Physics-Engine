#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float x, float y, float z, float fov, float windowWidth, float windowHeight)
	: Camera(x, y, z)
	, fieldOfView{fov}
{
	projection = glm::perspective(
		fieldOfView,
		windowWidth / windowHeight,
		0.1f,
		100.0f
	);

	updateCenter(glm::vec3(0.f, -350.f, 0.f));
}