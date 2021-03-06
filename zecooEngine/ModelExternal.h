#ifndef MODEL_H
#define MODEL_H

#include "stb_image.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshExternal.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Transform.h";
#include "Material.h";
#include "Texture.h"

using namespace std;

unsigned int TextureFromFile( const char* path, const string& directory, bool gamma = false );

class ModelExternal
{

public:
    std::string name = "";
    Transform* transform;
    Texture* texture;

    // model data 
    vector<MeshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<MeshExternal>    meshes;
    string directory;
    bool gammaCorrection;

    ModelExternal( string const& path, bool gamma = false );
    void Draw( Material* material, Texture* texture );

private:
    Material* material;
    void loadModel( string const& path );

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode( aiNode* node, const aiScene* scene );

    MeshExternal processMesh( aiMesh* mesh, const aiScene* scene );

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<MeshTexture> loadMaterialTextures( aiMaterial* mat, aiTextureType type, string typeName );
};

#endif