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

class CylinderMesh
{
private:
    float PI = 3.14;

    //offset values
    float xo = 0.0;
    float yo = 0.0;
    float zo = 0.0;

    float radius = 1.0;

    float height = 2;

    int pointCount = 30;
    int totalVertices = 0;

    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;

    int sectorCount = 10;

    vector<int> indices;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    std::vector<float> getUnitCircleVertices();

    CylinderMesh();
    void Render();
};

