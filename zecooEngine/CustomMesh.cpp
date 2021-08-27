#include "CustomMesh.h"
#include "math.h"

CustomMesh::CustomMesh()
{
    float angle = 0;

    int k = 0;
    for (int i = 0; i < pointCount; i++)
    {
        //divide the circle into angles acc to given points
        angle = 2.0 * i * (PI / pointCount);

        //defining vertex points accorcing to cos and sin angles
        circlePoints[k] =   xo + cos(angle) * rad;
        circlePoints[k+1] = yo + sin(angle) * rad;
        circlePoints[k+2] = zo;

        k += 3;
    }

    k = 0;
    int p = 0;

    for (size_t i = 0; i < (totalVertices-2); i+=3)
    {
        //we need to set all vertices to zero, 
        //setting it at cicle center
        vertices[i] = xo;
        vertices[i+1] = yo;
        vertices[i+2] = 2;
    }

    for (int i = 0; i < (totalVertices - 3); i+=3)
    {
        //we have used p to skip one value, to keep it at zero or given offset
        if (p != 0)
        {
            vertices[i] = circlePoints[k];
            vertices[i + 1] = circlePoints[k + 1];
            vertices[i + 2] = circlePoints[k + 2];
            k += 3;
        }

        p++;

        //reset the p after every 2 values
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
    glDrawArrays(GL_TRIANGLES, 0, (totalVertices/3));
}