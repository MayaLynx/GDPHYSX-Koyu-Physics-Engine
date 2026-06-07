#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	float x, y, z;

	//Setup attributes
	glm::vec3 cameraPos;
	glm::mat4 cameraPositionMatrix;
	glm::vec3 worldUp;
	glm::vec3 center;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 cameraRotMatrix;
	glm::mat4 viewMatrix;

protected:
	glm::mat4 projection;

public:
	Camera(float x, float y, float z);
	void setCameraPos(float x, float y, float z);
	glm::vec3 getCameraPos();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjection();
	void updateCenter(glm::vec3 newCenter);
	void updatePosition(glm::vec3 newPosition);
	void updateRotation(float theta);
};