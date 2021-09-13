#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

class LineMesh
{
private:
    float vertices[6] =
    {
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glm::vec3 pointA;
    glm::vec3 pointB;

    unsigned int VBO;
    unsigned int VAO;

public:
    LineMesh();
    void setPoints(glm::vec3 _pointA, glm::vec3 _pointB);
    void Render();
};


