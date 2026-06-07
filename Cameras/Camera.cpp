#include "Camera.h"

Camera::Camera(float x, float y, float z) : x{ x }, y{ y }, z{ z }
{
	// Camera position
	cameraPos = glm::vec3(x, y, z);
	cameraPositionMatrix =
		glm::translate(glm::mat4(1.0f),
			cameraPos * -1.0f);

	worldUp = glm::vec3(0, 1.0f, 0);
	center = glm::vec3(0, 0, 0);

	forward = glm::vec3(center - cameraPos);
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	// Camera matrix
	cameraRotMatrix = glm::mat4(1.0f);
	cameraRotMatrix[0][0] = right.x;
	cameraRotMatrix[1][0] = right.y;
	cameraRotMatrix[2][0] = right.z;

	cameraRotMatrix[0][1] = up.x;
	cameraRotMatrix[1][1] = up.y;
	cameraRotMatrix[2][1] = up.z;

	cameraRotMatrix[0][2] = -forward.x;
	cameraRotMatrix[1][2] = -forward.y;
	cameraRotMatrix[2][2] = -forward.z;

	// View matrix
	viewMatrix = cameraRotMatrix * cameraPositionMatrix;
}

void Camera::setCameraPos(float x, float y, float z)
{
	cameraPos = glm::vec3(x, y, z);
}

glm::vec3 Camera::getCameraPos()
{
	return cameraPos;
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjection()
{
	return projection;
}

// Moves the camera view using the mouse
void Camera::updateCenter(glm::vec3 newCenter)
{
	center = newCenter;

	forward = glm::vec3(center - cameraPos);
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	cameraRotMatrix[0][0] = right.x;
	cameraRotMatrix[1][0] = right.y;
	cameraRotMatrix[2][0] = right.z;

	cameraRotMatrix[0][1] = up.x;
	cameraRotMatrix[1][1] = up.y;
	cameraRotMatrix[2][1] = up.z;

	cameraRotMatrix[0][2] = -forward.x;
	cameraRotMatrix[1][2] = -forward.y;
	cameraRotMatrix[2][2] = -forward.z;

	viewMatrix = cameraRotMatrix * cameraPositionMatrix;
}

void Camera::updatePosition(glm::vec3 newPosition)
{
	// Update camera position
	cameraPos += newPosition;

	// Update the view matrix
	cameraPositionMatrix =
		glm::translate(glm::mat4(1.0f),
			cameraPos * -1.0f);
	viewMatrix = cameraRotMatrix * cameraPositionMatrix;
}

void Camera::updateRotation(float theta)
{
	cameraRotMatrix = glm::rotate(
		cameraRotMatrix,
		glm::radians(-theta),
		glm::normalize(glm::vec3(0.f, 1.f, 0.f))
	);

	viewMatrix = cameraRotMatrix * cameraPositionMatrix;
}