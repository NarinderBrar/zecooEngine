#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class PlaneMesh
{
private:
    float vertices[32] = {
        // position              normal          uv
         -0.5f,  0.0f, -0.5f,    0.0, 1.0, 0.0,  1.0, 1.0, // top right //0
          0.5f,  0.0f, -0.5f,    0.0, 1.0, 0.0,  0.0, 1.0, // top left  //1 
        - 0.5f,  0.0f,  0.5f,    0.0, 1.0, 0.0,  1.0, 0.0, // bottom right //2
          0.5f,  0.0f,  0.5f,    0.0, 1.0, 0.0,  0.0, 0.0, // bottom left // 3
    };

    unsigned int indices[6] = 
    {
        0, 1, 2, // first triangle
        1, 3, 2,  // second triangl
    };

    unsigned int VBO, VAO, EBO;

public:
    PlaneMesh();
    void Render();
    ~PlaneMesh();
};

