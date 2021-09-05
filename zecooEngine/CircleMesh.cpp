#include "CircleMesh.h"
#include "math.h"

CircleMesh::CircleMesh()
{
    //Center of the circle will be at 0,0,0
    vertices.push_back(xo);
    vertices.push_back(yo);
    vertices.push_back(zo);

    //cout << 0 << " x : " << vertices[0] << " y: " << vertices[1] << " z: " << vertices[2] << endl;

    for (int i = 0; i < pointCount; i++)
    {
        //divide the circle into angles acc to given points
        float angle = 2.0 * i * (PI / pointCount);

        //defining vertex points accorcing to cos and sin angles
        vertices.push_back(xo + cos(angle) * rad);
        vertices.push_back(yo);
        vertices.push_back(zo + sin(angle) * rad);
    }

    totalVertices = vertices.size();

    for (size_t i = 0; i < double(pointCount)-1; i++)
    {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }

    //Need to close the shape
    indices.push_back(0);
    indices.push_back(pointCount);
    indices.push_back(1);

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CircleMesh::Render()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_STRIP, totalVertices, GL_UNSIGNED_INT, 0);
}