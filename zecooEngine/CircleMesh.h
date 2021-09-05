#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>

using namespace std;

class CircleMesh
{
private:
    float PI = 3.14;

    //offset values
    float xo = 0.0;
    float yo = 0.0;
    float zo = 0.0;

    float rad = 1.0;

    int pointCount = 30;
    int totalVertices = 0;

    vector<float> vertices;
    vector<int> indices;

    //unsigned int indices[138];

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    CircleMesh();
    void Render();
};

