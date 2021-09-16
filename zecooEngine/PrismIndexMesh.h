#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class PrismIndexMesh
{
private:
    float vertices[18] =
    {
        1.0f,  1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
         -1.0f,  -1.0f, 0.0f,
      -1.0f, 1.0f, 0.0f,
     -1.0f,  0.0f, 1.0f,
       1.0f,  0.0f, 1.0f,
    };

    unsigned int indices[21] =
    {
       // 2,3,4,
        0,5,1,
        0,5,4,
        0,3,4,
        0,2,3,
          0,2,1,
          0,2,4,
          2,4,1
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    PrismIndexMesh();
    void Render();
};
