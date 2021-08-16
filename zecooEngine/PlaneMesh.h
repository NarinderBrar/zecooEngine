#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class PlaneMesh
{
private:
    float vertices[32] = {
        // positions          // normals           // texture coords
         0.5f,  0.0f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.0f, 0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;

public:
    PlaneMesh();
    void Render();
};

