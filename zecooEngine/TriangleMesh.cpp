#include "TriangleMesh.h"
#include "math.h"

TriangleMesh::TriangleMesh ()
{
    DrawLoop(0, 0, 20, 0.1, 0.5);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    //Define an array of generic vertex attribute data
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void TriangleMesh::DrawLoop(float centerX, float centerY, float sides, float innerRadius, float outerRadius) 
{    
    for (int i = 0; i <= sides; i += 4) 
    {
        verticies[i + 0] = centerX + (sin(glm::radians(360.0f * (i / sides))) * innerRadius);
        verticies[i + 1] = centerY - (cos(glm::radians(360.0f * (i / sides))) * innerRadius);
        verticies[i + 2] = centerX + (sin(glm::radians(360.0f * (i / sides))) * outerRadius);
        verticies[i + 3] = centerY - (cos(glm::radians(360.0f * (i / sides))) * outerRadius);
    }
}


void TriangleMesh::Render()
{
    // render container
    glBindVertexArray(VAO);

    //glDrawArrays — render primitives from array data
    glDrawArrays(GL_LINE_LOOP, 0, 27);
}