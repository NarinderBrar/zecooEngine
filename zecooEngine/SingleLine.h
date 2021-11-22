#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include <vector>
using namespace std;

class SingleLine
{
    int shaderProgram;
    unsigned int VBO, VAO;

    vector<float> vertices;

    glm::vec3 startPoint;
    glm::vec3 endPoint;

public:
    SingleLine( glm::vec3 start, glm::vec3 end );
    int setMVP( glm::mat4 mvp );
    int setColor( glm::vec3 color );
    void Update( glm::vec3 start, glm::vec3 end );
    int draw();
    ~SingleLine();
};