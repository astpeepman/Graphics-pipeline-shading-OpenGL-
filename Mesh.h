#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>

using namespace std;

struct Vertex {
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
};

struct texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<texture>      textures;
    unsigned int VAO;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<texture> textures);
    void Draw(Shader shader);
private:
    /*  Render data  */
    unsigned int VBO, EBO;
    /*  Functions    */
    void setupMesh();
};

