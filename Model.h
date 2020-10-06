#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dMath.h"

#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <Windows.h>

using namespace std;
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    vec3 position;
    vector<texture> textures_loaded;	
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    
    Model() {};
    
    Model(string const& path, vec3 pos, Shader s, bool gamma = false) : gammaCorrection(gamma)
    {
        model = mat4(1.0f);
        shader = s;
        position = pos;

        loadModel(path);

        model=glm::translate(model, position);
    }

    
    virtual void Draw()
    {
        shader.setMat4("model", model);

        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    void Rotate(GLfloat angle, vec3 axis);
    void Scale(vec3 sc);
    void trans(vec3 tr);
    void setproj(mat4 prj);
    void setview(mat4 view);

protected :
    mat4 model;
    Shader shader;
    /*  Ìועמהû   */
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
   
};

