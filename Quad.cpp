#include "Quad.h"
#include <vector>

unsigned int loadTexture(char const* path);

Quad::Quad()
{
}

Quad::Quad(vec3 pos, GLfloat height, GLfloat width, material mtl, Shader s) {
	position = pos;
	
    diff_tex = mtl.diffuse;
    spec_tex = mtl.specular;
    
    shader = s;


    model = mat4(1.0f);

    std::vector<GLfloat> vertex;
    vertex.push_back(-width / 2);
    vertex.push_back(-height / 2);
    vertex.push_back(0);
    vertex.push_back(-width / 2);
    vertex.push_back(height / 2);
    vertex.push_back(0);
    vertex.push_back(width / 2);
    vertex.push_back(height / 2);
    vertex.push_back(0);
    vertex.push_back(width / 2);
    vertex.push_back(-height / 2);
    vertex.push_back(0);

    std::vector<GLfloat> normals;
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    std::vector<GLfloat> texCoords;
    texCoords.push_back(0);
    texCoords.push_back(0);
    texCoords.push_back(0);
    texCoords.push_back(20.0f);
    texCoords.push_back(20.0f);
    texCoords.push_back(20.0f);
    texCoords.push_back(20.0f);
    texCoords.push_back(0);

    std::vector<int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);


    model = glm::translate(model, position);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &indicesVBO);
    glBindVertexArray(VAO);


    //position
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    //texCoord
    glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);


    //DiffTexture = loadTexture();
    
    glGenTextures(1, &DiffTexture);
    glBindTexture(GL_TEXTURE_2D, DiffTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, diff_tex.width, diff_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, diff_tex.image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(diff_tex.image);
    glBindTexture(GL_TEXTURE_2D, DiffTexture);


    glGenTextures(1, &SpecTexture);
    glBindTexture(GL_TEXTURE_2D, SpecTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spec_tex.width, spec_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, spec_tex.image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(spec_tex.image);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);

    if (mtl.normal.height == 0 || mtl.normal.width == 0) {
        nrmFlg = false;
    }
    else {
        norm_tex = mtl.normal;

        glGenTextures(1, &BumpTexture);
        glBindTexture(GL_TEXTURE_2D, BumpTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load, create texture and generate mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, norm_tex.width, norm_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, norm_tex.image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(norm_tex.image);
        glBindTexture(GL_TEXTURE_2D, BumpTexture);
        nrmFlg = true;
    }

    //текстурцы в шейдер
    shader.Use();
    shader.setFloat("material.shininess", mtl.shininess);


    indicSize = indices.size();


}


void Quad::draw() {
    
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("material.diffuseMap1", 0);
    glBindTexture(GL_TEXTURE_2D, DiffTexture);

    glActiveTexture(GL_TEXTURE1);
    shader.setInt("material.specularMap1", 1);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);

    if (nrmFlg) {
        glActiveTexture(GL_TEXTURE2);
        shader.setInt("material.normalMap1", 2);
        glBindTexture(GL_TEXTURE_2D, BumpTexture);
    }

    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicSize, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Quad::Rotate(GLfloat angle, vec3 axis)
{
    model = glm::rotate(model, radians(angle), axis);
}

void Quad::translate(vec3 cubepos)
{
    model = glm::translate(model, cubepos);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}