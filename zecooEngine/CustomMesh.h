#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include <iostream>
using namespace std;

class CustomMesh
{
private:
    int pointCount = 30;
    float circlePoints[90]; // pointCount*3
    float vertices[136];// circlePoints + circlePoints/2

    unsigned int VBO;
    unsigned int VAO;

public:
    CustomMesh();
    void DrawLoop(float centerX, float centerY, float sides, float innerRadius, float outerRadius);
    void Render();
};

