#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

class TriangleMesh
{
    private:
        float vertices[18] = {
           -0.1f, -0.1f, 0.0f, //left
            0.1f, -0.1f, 0.0f,  //right
            0.0f,  0.1f, 0.0f,   //top
           -0.5f, -0.1f, 0.0f, //left
           -0.3f, -0.1f, 0.0f,  //right
            -0.4f,  0.1f, 0.0f,   //top
        };

        unsigned int VBO;
        unsigned int VAO;

	public:
		TriangleMesh();
        void DrawLoop(float centerX, float centerY, float sides, float innerRadius, float outerRadius);
        void Render();
};

