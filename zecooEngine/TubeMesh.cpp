#include "TubeMesh.h"
#include "math.h"

std::vector<float> TubeMesh::getCircleVertices(glm::vec3 origin, int points, float rad)
{
    std::vector<float> circleVertices;

    for (int i = 0; i < points; i++)
    {
        //divide the circle into angles acc to given points
        float angle = 2.0 * i * (PI / points);

        //defining vertex points accorcing to cos and sin angles
        circleVertices.push_back(origin[1]);
        circleVertices.push_back(origin[0] + cos(angle) * rad);
        circleVertices.push_back(origin[2] + sin(angle) * rad);
    }

    return circleVertices;
}

TubeMesh::TubeMesh()
{
    std::vector<float> circleVerticesTop = getCircleVertices(glm::vec3(0, height / 2, 0), pointCount, 1.0f);
    std::vector<float> circleVerticesBottom = getCircleVertices(glm::vec3(0, -height / 2, 0), pointCount, 1.0f);
    std::vector<float> circleVerticesInnerTop = getCircleVertices(glm::vec3(0, height / 2, 0), pointCount, 0.8f);
    std::vector<float> circleVerticesInnerBottom = getCircleVertices(glm::vec3(0, -height / 2, 0), pointCount, 0.8f);

    for (int i = 0; i < circleVerticesTop.size(); i++)
    {
        positions.push_back(circleVerticesTop[i]);
    }
    for (int i = 0; i < (circleVerticesTop.size() / 3)-2; i+=3)
    {
        normals.push_back(circleVerticesTop[i]);
        normals.push_back(circleVerticesTop[i+1]);
        normals.push_back(circleVerticesTop[i+2]);
    }

    /*for (int i = 0; i < circleVerticesTop.size(); i++)
    {
        positions.push_back(circleVerticesTop[i]);
    }
    for (int i = 0; i < circleVerticesTop.size()/3; i++)
    {
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(1);
    }*/

    for (int i = 0; i < circleVerticesBottom.size(); i++)
    {
        positions.push_back(circleVerticesBottom[i]);
    }
    for (int i = 0; i < circleVerticesBottom.size() / 3; i++)
    {
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(-1);
    }

    for (int i = 0; i < circleVerticesInnerTop.size(); i++)
    {
        positions.push_back(circleVerticesInnerTop[i]);
    }
    for (int i = 0; i < circleVerticesInnerTop.size() / 3; i++)
    {
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(-1);
    }

    for (int i = 0; i < circleVerticesInnerBottom.size(); i++)
    {
        positions.push_back(circleVerticesInnerBottom[i]);
    }
    for (int i = 0; i < circleVerticesInnerBottom.size() / 3; i++)
    {
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(-1);
    }

    
    for (int i = 0, k = 0; i < positions.size()/3; i++, k+=3)
    {
        vertices.push_back(positions[k]);
        vertices.push_back(positions[k+1]);
        vertices.push_back(positions[k+2]);

        vertices.push_back(normals[k]);
        vertices.push_back(normals[k+1]);
        vertices.push_back(normals[k+2]);
    }

    totalVertices = positions.size();
    int k;
    int l;
    k = pointCount;
    for (int i = 0; i < pointCount - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(k);
        indices.push_back(k + 1);
        k++;
    }
    indices.push_back(pointCount-1);
    indices.push_back((pointCount * 2) - 1);
    indices.push_back(pointCount);

    k = pointCount;
    for (int i = 0; i < pointCount - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(k + 1);
        k++;
    }
    indices.push_back(0);
    indices.push_back(pointCount - 1);
    indices.push_back(pointCount);

    
    l = pointCount * 3;
    for (int i = pointCount*2; i < (pointCount * 3) - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(l);
        indices.push_back(l + 1);
        l++;
    }
    indices.push_back((pointCount*3)-1);
    indices.push_back((pointCount * 4) - 1);
    indices.push_back(pointCount*3);

    l = pointCount * 3;
    for (int i = pointCount * 2; i < (pointCount * 3) - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(l + 1);
        l++;
    }
    indices.push_back(pointCount * 2);
    indices.push_back((pointCount * 3) - 1);
    indices.push_back(pointCount * 3);
    
    k = pointCount * 2;
    for (int i = 0; i < pointCount - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(k);
        k++;
    }
    indices.push_back(pointCount - 1);
    indices.push_back(0);
    indices.push_back((pointCount * 3)-1);

    k = pointCount * 2;
    for (int i = 0; i < pointCount - 1; i++)
    {
        indices.push_back(i + 1);
        indices.push_back(k);
        indices.push_back(k + 1);
        k++;
    }
    indices.push_back(0);
    indices.push_back(pointCount * 2);
    indices.push_back((pointCount * 3) - 1);

    l = pointCount * 3;
    for (int i = pointCount; i < (pointCount * 2) - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(l + 1);
        l++;
    }
    indices.push_back((pointCount * 2) - 1);
    indices.push_back(pointCount);
    indices.push_back(pointCount * 3);

    l = pointCount * 3;
    for (int i = pointCount; i < (pointCount * 2) - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(l);
        indices.push_back(l + 1);
        l++;
    }
    indices.push_back((pointCount * 2) - 1);
    indices.push_back((pointCount * 4) - 1);
    indices.push_back(pointCount * 3);

    debugger->printMsg(std::to_string(positions.size()));
    debugger->printMsg(std::to_string(normals.size()));

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
}

void TubeMesh::Render()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}