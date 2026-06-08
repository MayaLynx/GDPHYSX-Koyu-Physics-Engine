#include "model3D.h"


model3D::model3D(std::string path, glm::vec3 position, GLuint shader)
    : path{ path }
    , position{ position }
    , shader{ shader }
{
    scale_x = scale_y = scale_z = 1.f;
    axis_x = axis_y = 0.f;
    axis_z = 1.f;
    theta = 0.f;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        model3D::path.c_str()
    );

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    transformation_matrix = glm::mat4(1.0f);

    // std::cout << "Object created" << std::endl;
}

void model3D::setColor(glm::vec3 newColor)
{
    color = newColor;
}

void model3D::updatePosition(glm::vec3 newPos)
{
    position = newPos;
}

void model3D::translate()
{
    transformation_matrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(position)

    );
}

void model3D::setScale(float x, float y, float z)
{
    scale_x = x;
    scale_y = y;
    scale_z = z;
}

void model3D::scale()
{
    transformation_matrix = glm::scale(
        transformation_matrix,
        glm::vec3(scale_x, scale_y, scale_z)
    );
}

void model3D::rotate()
{
    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta),
        glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
    );
}

void model3D::transform()
{
    translate();
    scale();
    rotate();

    unsigned int transformLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(transformation_matrix));
}

void model3D::draw()
{
    glUseProgram(shader);
    glBindVertexArray(VAO);

    transform();

    unsigned int colorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    glDrawElements(GL_TRIANGLES,
        mesh_indices.size(),
        GL_UNSIGNED_INT,
        0);
}