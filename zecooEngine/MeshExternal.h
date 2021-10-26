#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "Shader.h"
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture
{
    unsigned int id;
    string type;
    string path;
};

class MeshExternal
{
public:
    // mesh Data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<MeshTexture>  textures;

    unsigned int VAO;
    unsigned int VBO, EBO;

public:
    MeshExternal( vector<Vertex> vertices, vector<unsigned int> indices, vector<MeshTexture> textures );

    void Draw( Shader& shader );
    void setupMesh();
};
#endif