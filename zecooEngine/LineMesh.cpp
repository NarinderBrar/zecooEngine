#include "LineMesh.h"
#include "math.h"

LineMesh::LineMesh()
{
    glLineWidth(2.0);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LineMesh::setPoints(glm::vec3 _pointA, glm::vec3 _pointB)
{
    pointA = _pointA;
    pointB = _pointB;
}

void LineMesh::Render()
{
    vertices[0] = pointA.x;
    vertices[1] = pointA.y;
    vertices[2] = pointA.z;
    
    vertices[3] = pointB.x;
    vertices[4] = pointB.y;
    vertices[5] = pointB.z;
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINE_STRIP, 0, 2);
}