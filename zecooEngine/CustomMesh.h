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

class CustomMesh
{
private:
    float PI = 3.14;

    float radius = 1.0;
    float sectorCount = 10;
    float stackCount = 10;

    vector<float> vertices;
    vector<int> indices;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    CustomMesh();
    void Render();
};

