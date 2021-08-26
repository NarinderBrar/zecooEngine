#include "CustomMesh.h"
#include "math.h"

CustomMesh::CustomMesh()
{
    float angle = 0;
    float PI = 3.14;

    float xo = 0;
    float yo = 0;

    float rad = 0.5;

    int k = 0;
    for (int i = 0; i < pointCount; i++)
    {
        angle = 2.0 * i * (PI / pointCount);

        //circlePoints[0] = x
        //circlePoints[1] = y
        //circlePoints[2] = z

        circlePoints[k] = cos(angle) * rad;
        circlePoints[k+1] = sin(angle) * rad;
        circlePoints[k+2] = 0.0;

        k += 3;
    }

    k = 0;
    int p = 0;

    //i must be increment by 3 not 1
    for (int i = 0; i < 133; i+=3)
    {
        vertices[i] = 0;
        vertices[i+1] = 0;
        vertices[i+2] = 0;

        if (p != 0)
        {
            vertices[i] = circlePoints[k];
            vertices[i + 1] = circlePoints[k + 1];
            vertices[i + 2] = circlePoints[k + 2];
            k += 3;
        }

        p++;
        if (p == 3)
            p = 0;
    }

    //glGenBuffers returns n buffer object names in buffers.
    glGenBuffers(1, &VBO);

    //glGenVertexArrays returns n vertex array object names in arrays.
    glGenVertexArrays(1, &VAO);

    //glBindVertexArray binds the vertex array object with name array
    //array is the name of a vertex array object previously returned from a call to glGenVertexArrays
    glBindVertexArray(VAO);

    //glBindBuffer binds a buffer object to the specified buffer binding point
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copy vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Define an array of generic vertex attribute data
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void CustomMesh::Render()
{
    // render container
    glBindVertexArray(VAO);

    //glDrawArrays — render primitives from array data
    glDrawArrays(GL_TRIANGLES, 0, 45);
}