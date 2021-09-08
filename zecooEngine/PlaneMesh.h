#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class PlaneMesh
{
private:
    float vertices[48] = {
        // position            normal           uv
         -0.5f,  0.0f, -0.5f,    0.0, 1.0, 0.0,  1.0, 1.0, // top right //0
          0.5f,  0.0f, -0.5f,    0.0, 1.0, 0.0,  0.0, 1.0, // top left  //1 
        - 0.5f,  0.0f,  0.5f,   0.0, 1.0, 0.0,  1.0, 0.5, // bottom right //2
          0.5f,  0.0f,  0.5f,   0.0, 1.0, 0.0,  0.0, 0.5, // bottom left // 3

       - 0.5f,  0.0f, 1.5f,    0.0, 1.0, 0.0,  1.0, 0.0, // bottom right // 4
         0.5f,  0.0f, 1.5f,    0.0, 1.0, 0.0,  0.0, 0.0 // bottom left // 5
    };

    unsigned int indices[12] = 
    {
        0, 1, 2, // first triangle
        1, 3, 2,  // second triangl
        2,3,4,
        3,5,4
    };

    unsigned int VBO, VAO, EBO;

public:
    PlaneMesh();
    void Render();
};

