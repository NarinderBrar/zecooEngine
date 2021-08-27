#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class TriangleIndexMesh
{
private:
    float vertices[12] =
    {
        0.0f,  0.5f, 0.0f,   //top //0
      - 0.5f, -0.5f, 0.0f, //left //1
        0.5f, -0.5f, 0.0f,  //right//2
        0.5f, 0.5f, 0.0f  //extra
    };

    unsigned int indices[6] = 
    {
        0, 1, 2, // first triangle
        0, 2, 3 // second triangle
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    TriangleIndexMesh();
    void Render();
};

