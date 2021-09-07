#include "ConeMesh.h"
#include <cmath>

ConeMesh::ConeMesh()
{
    float angle = 0;
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    int k = 3;
    for (int i = 0; i < pointCount; i++)
    {
        //divide the circle into angles acc to given points
        angle = 2.0 * i * (PI / pointCount);

        //defining vertex points accorcing to cos and sin angles
        vertices[k] = xo + cos(angle) * rad;
        vertices[k + 1] = yo;
        vertices[k + 2] = zo + sin(angle) * rad;
        k += 3;
    }

    vertices[totalVertices - 3] = 0.0f;
    vertices[totalVertices - 2] = 2.0f;
    vertices[totalVertices - 1] = 0.0f;

    int y = 1, x = 1, z, w;
    for (int i = 0; i < indexCount; i += 3)
    {
        if (i < indexCount / 2)
        {
            z = y + 1;
            if (z > pointCount)
                z = 1;
            indices[i] = 0;
            indices[i + 1] = y;
            indices[i + 2] = z;

            y++;
        }
        else
        {
            w = x + 1;
            if (w > pointCount)
                w = 1;
            indices[i] = 41;
            indices[i + 1] = x;
            indices[i + 2] = w;

            x++;
        }
    }

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void ConeMesh::Render()
{
    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}