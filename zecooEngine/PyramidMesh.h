
#pragma once
#include <glad/glad.h>
#include <glfw3.h>

class PyramidMesh
{
private:
    float vertices[15] =
    {
        0.0f,  1.0f, 0.0f,
      -1.0f,  0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f
    };

    unsigned int indices[12] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    PyramidMesh();
    void Render();
};

