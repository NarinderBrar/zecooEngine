#include "CylinderMesh.h"
#include "math.h"

std::vector<float> CylinderMesh::getCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectors;
    float sectorAngle;

    std::vector<float> circleVertices;

    for (int i = 0; i <= sectors; ++i)
    {
        sectorAngle = i * sectorStep;

        circleVertices.push_back(cos(sectorAngle));
        circleVertices.push_back(sin(sectorAngle));
        circleVertices.push_back(0);
    }
    return circleVertices;
}


CylinderMesh::CylinderMesh()
{
    std::vector<float> unitVertices = getCircleVertices();

    // Side vertices
    //i = 0 for bottom circle, i = 1 for top circle
    for (int i = 0; i < 2; ++i)
    {
        //height = -h/2 to h/2
        float h = -height / 2.0f + i * height;

        //texture coordinates t = 1 to 0
        float t = 0.5f - i*0.5f;

        for (int j = 0, k = 0; j <= sectors; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            float uz = unitVertices[k + 2];

            // position vector
            circleVertices.push_back(ux * radius); 
            circleVertices.push_back(uy * radius); 
            circleVertices.push_back(h);   

            // normal vector
            normals.push_back(ux);             
            normals.push_back(uy);                 
            normals.push_back(uz);              

            // texture coordinate
            //s -> start with 0
            texCoords.push_back((float)j / sectors);
            //t -> start with 1
            //(0,1) first coordinate on top left
            texCoords.push_back(t);                 
        }
    }

    int baseCenterIndex = (int)circleVertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectors + 1;

    //debugger->printMsg(glm::to_string(baseCenterIndex));
    //debugger->printMsg(glm::to_string(topCenterIndex));

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        // z value; -h/2 to h/2
        float h = -height / 2.0f + i * height;  

        // z value of normal; -1 to 1
        float nz = -1 + i * 2;                           

        // center point
        circleVertices.push_back(0);     
        circleVertices.push_back(0);     
        circleVertices.push_back(h);

        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(nz);

        if (i == 0)
        {
            texCoords.push_back(0.75f);
            texCoords.push_back(0.75f);
        }
        else
        {
            texCoords.push_back(0.25f);
            texCoords.push_back(0.75f);
        }

        for (int j = 0, k = 0; j < sectors; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];

            // position vector
            circleVertices.push_back(ux * radius);
            circleVertices.push_back(uy * radius);
            circleVertices.push_back(h);

            // normal vector
            normals.push_back(0);
            normals.push_back(0); 
            normals.push_back(nz);

           if (i == 0)
           {
                // texture coordinate
                texCoords.push_back(ux * 0.25f + 0.75f);
                texCoords.push_back(uy * 0.25f + 0.75f);
           }
           else
            {
                texCoords.push_back(ux * 0.25f + 0.25f);
                texCoords.push_back(uy * 0.25f + 0.75f);
            }
        }
    }

    // 1st vertex index at base
    int k1 = 0;

    // 1st vertex index at top
    int k2 = sectors + 1;           

    for (int v = 0, t = 0; v < circleVertices.size() ; v+=3, t+=2)
    {
        vertices.push_back(circleVertices[v]);
        vertices.push_back(circleVertices[v+1]);
        vertices.push_back(circleVertices[v+2]);

        vertices.push_back(normals[v]);
        vertices.push_back(normals[v+1]);
        vertices.push_back(normals[v+2]);

        vertices.push_back(texCoords[t]);
        vertices.push_back(texCoords[t+1]);
    }


    // indices for the side surface
    for (int i = 0; i < sectors; ++i, ++k1, ++k2)
    {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        indices.push_back(k1);
        indices.push_back(k1 + 1);
        indices.push_back(k2);

        // k2 => k1+1 => k2+1
        indices.push_back(k2);
        indices.push_back(k1 + 1);
        indices.push_back(k2 + 1);
    }

    // indices for the base surface
   for (int i = 0, k = baseCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(k + 1);
            indices.push_back(k);
        }
        else // last triangle
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(baseCenterIndex + 1);
            indices.push_back(k);
        }
    }

    // indices for the top surface
    for (int i = 0, k = topCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(k + 1);
        }
        else // last triangle
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(topCenterIndex + 1);
        }
    }

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);
}

void CylinderMesh::Render()
{
    glBindVertexArray(VAO);
    //glDrawElements(GL_LINE_STRIP, totalVertices+150, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}