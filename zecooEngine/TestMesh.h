#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class TestMesh
{
private:
    float vertices[24] =
    {
        // positions          // normals           // texture coords
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //left
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, //right
        0.0f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f   //top
    };

    unsigned int VBO, VAO;

public:
    TestMesh();
    void Render();
};

