#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include "Debugger.h"

using namespace std;

class CylinderMesh
{
private:
    float radius = 1.0;
    float height = 2;

    vector<float> tmpVertices;

    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;

    int sectorCount = 50;

    vector<int> indices;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    Debugger debugger;

public:
    std::vector<float> getUnitCircleVertices();

    CylinderMesh();
    void Render();
};

