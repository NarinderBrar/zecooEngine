#include "BtConvexHullShapeBlenderMesh.h"
#include "LoaderForGraph.h"
BtConvexHullShapeBlenderMesh::BtConvexHullShapeBlenderMesh(string s1, PointForEq* p)
{
    vector<float> v, n;
    vector<int> i;
    LoaderForGraph* l = new LoaderForGraph(s1);
    l->LoadMe(NULL, v, n, uvs, i,p);
    int z = 0;
    for (int i = 3;i < v.size();i += 3, z += 2)
    {
        vertices.push_back(v[i]); vertices.push_back(v[i + 1]); vertices.push_back(v[i + 2]);
        vertices.push_back(n[i]); vertices.push_back(n[i + 1]); vertices.push_back(n[i + 2]);
        vertices.push_back(uvs[z]); vertices.push_back(uvs[z + 1]);
     //  cout<<"................" << " x " << v[i] << " y " << v[i + 1] << " z " << v[i + 2] << " a " << uvs[z] << " b " << uvs[z + 1] << endl; 
    }
    /* for (int i = 0;i < v.size();i++)
     {
         vertices[i] = v[i];
     }
     size = v.size()/3;*/
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

     //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void BtConvexHullShapeBlenderMesh::Render()
{
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 3);
}