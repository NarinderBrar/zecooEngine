#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "Debugger.h"

#include <string> 


#include <iostream>
#include <vector>

using namespace std;

class CylinderMesh
{
private:
    float radius = 1.0;
    float height = 2;
    int sectors = 30;

    vector<float> circleVertices;

    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;

    vector<int> indices;

    Debugger* debugger;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    std::vector<float> getCircleVertices();

    CylinderMesh();
    void Render();
};

