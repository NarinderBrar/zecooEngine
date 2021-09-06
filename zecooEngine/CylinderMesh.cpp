#include "CylinderMesh.h"
#include "math.h"

std::vector<float> CylinderMesh::getCircleVertices(glm::vec3 origin, int points)
{
    std::vector<float> circleVertices;

    //Center of the circle will be at 0,0,0
    circleVertices.push_back(origin[0]);
    circleVertices.push_back(origin[1]);
    circleVertices.push_back(origin[2]);

    for (int i = 0; i < points; i++)
    {
        //divide the circle into angles acc to given points
        float angle = 2.0 * i * (PI / points);

        //defining vertex points accorcing to cos and sin angles
        circleVertices.push_back(origin[0] + cos(angle) * rad);
        circleVertices.push_back(origin[1]);
        circleVertices.push_back(origin[2] + sin(angle) * rad);
    }

    return circleVertices;
}

CylinderMesh::CylinderMesh()
{
    std::vector<float> circleVerticesTop = getCircleVertices(glm::vec3(0, height/2,0), pointCount);
    std::vector<float> circleVerticesBottom = getCircleVertices(glm::vec3(0, -height/2, 0), pointCount);

    for (int i = 0; i < circleVerticesTop.size(); i++)
    {
        vertices.push_back(circleVerticesTop[i]);
    }

    for (int i = 0; i < circleVerticesBottom.size(); i++)
    {
        vertices.push_back(circleVerticesBottom[i]);
    }

    totalVertices = vertices.size();

    for (int i = 0; i < double(pointCount)-1; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        cout << " x : " << 0 << " y: " << i + 1 << " z: " << i + 2 << endl;
    }

    //Need to close the shape
    indices.push_back(0);
    indices.push_back(pointCount);
    indices.push_back(1);

    cout << " x : " << 0 << " y: " << pointCount << " z: " << 1 << endl << endl;

    for (int i = pointCount+1; i < (double(pointCount)*2); i++)
    {
        indices.push_back(pointCount+1);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        cout <<" x : " << pointCount + 1 << " y: " << i+1 << " z: " << i+2 << endl;
    }

    //Need to close the shape
    indices.push_back(pointCount + 1);
    indices.push_back(pointCount*2+1);
    indices.push_back(pointCount + 2);

    cout << " x : " << pointCount + 1 << " y: " << pointCount * 2 +1 << " z: " << pointCount + 2 << endl << endl;

    //example for 3 points
    //1 2 3 -> 0
    //5 6 7 -> 4

    // 1 5 6, 2 6 7, 3 7 8
    int k = pointCount;
    for (int i = 1; i < double(pointCount); i++)
    {
        indices.push_back(i);
        indices.push_back(k+2);
        indices.push_back(k+3);

        cout << " x : " << i << " y: " << k + 2 << " z: " << k + 3 << endl;

        k++;
    }

    //1 6 2, 2 7 3, 3 8 4
    k = pointCount;
    for (int i = 1; i < double(pointCount); i++)
    {
        indices.push_back(i);
        indices.push_back(k + 3);
        indices.push_back(i + 1);

        cout << " x : " << i << " y: " << k + 2 << " z: " << k + 3 << endl;

        k++;
    }

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CylinderMesh::Render()
{
    glBindVertexArray(VAO);
   glDrawElements(GL_LINE_STRIP, totalVertices+150, GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_TRIANGLES, totalVertices + 150, GL_UNSIGNED_INT, 0);
}