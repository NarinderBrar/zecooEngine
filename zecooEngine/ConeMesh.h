#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
using namespace std;

class ConeMesh
{
private:
    float PI = 3.14;

    //offset values
    float xo = 0.0;
    float yo = 0.0;
    float zo = 0.0;
    float rad = 1.0;

    int pointCount = 40;// No of points need on the circle circumference
    int totalVertices = 126;// (pointCount * 3) + 6; (+6 is for 2 extra points when center point and one top points)
    float vertices[126];

    int indexCount = 240;
    unsigned int indices[240];

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    ConeMesh();
    void Render();
};

