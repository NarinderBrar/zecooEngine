#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class TriangleIndexMesh
{
private: 
    float vertices[12] =
    {
        0.0f,  0.5f, 0.0f, 
      - 0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    unsigned int indices[6] = 
    {
        0, 1, 2,
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    TriangleIndexMesh();
    void Render();
};

