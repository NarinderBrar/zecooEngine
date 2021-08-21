#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class TriangleIndexMesh
{
private:
    float vertices[9] =
    {
        0.0f,  0.5f, 0.0f,   //top
      - 0.5f, -0.5f, 0.0f, //left
        0.5f, -0.5f, 0.0f,  //right
        //0.5f, 0.5f, 0.0f  //extra
    };
    unsigned int indices[3] = {
        0, 1, 2, // first triangle
        //0, 2, 3
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    TriangleIndexMesh();
    void Render();
};

