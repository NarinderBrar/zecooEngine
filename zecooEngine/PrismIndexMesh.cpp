#include "PrismIndexMesh.h"

PrismIndexMesh::PrismIndexMesh()
{
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

void PrismIndexMesh::Render()
{
    // render container
    glBindVertexArray(VAO);
   // glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINE_STRIP, 72, GL_UNSIGNED_INT, 0);
}