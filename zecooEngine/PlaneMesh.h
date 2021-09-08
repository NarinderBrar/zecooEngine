#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class PlaneMesh
{
private:
    float vertices[32] = {
        // position            normal           uv
         0.5f,  0.0f, 0.5f,    0.0, -1.0, 0.0,  1.0, 1.0, // top right
         0.5f,  0.0f, -0.5f,   0.0, -1.0, 0.0,  1.0, 0.0, // bottom right
        -0.5f,  0.0f, -0.5f,   0.0, -1.0, 0.0,  0.0, 0.0, // bottom left
        -0.5f,  0.0f, 0.5f,    0.0, -1.0, 0.0,  0.0, 1.0 // top left 
    };

    unsigned int indices[6] = 
    {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;

public:
    PlaneMesh();
    void Render();
};

