#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class TriangleMesh
{
    private:
        float vertices[9] =
        {
           -0.5f, -0.5f, 0.0f, //left
            0.5f, -0.5f, 0.0f,  //right
            0.0f,  0.5f, 0.0f,   //top
        };

        unsigned int VBO;
        unsigned int VAO;

	public:
		TriangleMesh();
        void Render();
};

