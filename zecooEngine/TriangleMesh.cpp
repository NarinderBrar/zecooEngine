#include "TriangleMesh.h"
#include "math.h"

TriangleMesh::TriangleMesh ()
{
    /*float angle = 0;
    float M_PI = 3.14;
    float xo = 0;
    float yo = 0;
    float rad = 0.5;
    
    int k = 0;

    cout << "hi";
    for (int i = 0; i < 100; i++) 
    {
        // Change the angle
        angle = i * 2 * (M_PI / 100);

        vertices[k] = (xo + (cos(angle) * rad));
        vertices[k+1] = (yo + (sin(angle) * rad));
        vertices[k+2] = 0.0f;

        cout << i;
        //cout << angle;
        //cout << k;
        cout << "\n";

        k+=3;
    }*/

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


void TriangleMesh::Render()
{
    // render container
    glBindVertexArray(VAO);

    //glDrawArrays — render primitives from array data
    glDrawArrays(GL_TRIANGLES, 0, 6);
}