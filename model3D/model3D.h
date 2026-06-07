#pragma once
#pragma once
#include <glad/glad.h>
#include "../tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>

class model3D
{
private:
	glm::vec3 position;
	float scale_x, scale_y, scale_z;
	float axis_x, axis_y, axis_z;
	float theta;
	std::string path;
	glm::vec3 color;
	GLuint VAO, VBO, EBO;
	GLuint shader;
	std::vector<GLuint> mesh_indices;
	glm::mat4 transformation_matrix;

public:
	model3D(std::string path, glm::vec3 position, GLuint shader);
	void setColor(glm::vec3 newColor);
	void updatePosition(glm::vec3 newPos);
	void translate();
	void setScale(float x, float y, float z);
	void scale();
	void rotate();
	void transform();
	void draw();
};