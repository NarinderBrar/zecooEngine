#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>
#include "Debugger.h"

#include <iostream>
#include <vector>

using namespace std;

class TubeMesh
{
private:
    float PI = 3.14;

    //offset values
    float xo = 0.0;
    float yo = 0.0;
    float zo = 0.0;

    float rad = 1.0;

    float height = 2;

    int pointCount = 5;
    int totalVertices = 0;

    vector<float> vertices;
    
    vector<float> positions;
    vector<float> normals;

    vector<int> indices;
    Debugger* debugger;
    
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    std::vector<float> getCircleVertices(glm::vec3 origin, int points, float rad);
    TubeMesh();
    void Render();

};

