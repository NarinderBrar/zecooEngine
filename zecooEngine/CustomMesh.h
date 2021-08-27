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
    float PI = 3.14;

    //offset values
    float xo = 0.0;
    float yo = 0.0;
    float zo = 0.0;

    float rad = 1.0;

    int pointCount = 30;
    float circlePoints[90];// pointCount*3
    int totalVertices = 136;
    float vertices[136];// circlePoints + circlePoints/2 + 1

    unsigned int VBO;
    unsigned int VAO;

public:
    CustomMesh();
    void DrawLoop(float centerX, float centerY, float sides, float innerRadius, float outerRadius);
    void Render();
};

