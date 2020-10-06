#include "Plane.h"
#include <vector>

Plane::Plane(vec3 pos, Shader s, Texture dt, Texture st, Texture nt)
{
	position = pos;
	shader = s;
	diff_tex = dt;
	spec_tex = st;
    norm_tex = nt;

	model = mat4(1.0f);
	model = translate(model, position);

    shader.Use();

    std::vector<GLfloat> vertex = { 0.0f,  0.0f,  0.0f,  1.0f,
                                       1.0f , 0.0f,  0.0f,  1.0f,
                                       0.0f,  0.0f, -1.0f, 1.0f,
                                       1.0f, 0.0f, -1.0f , 1.0f }; // четвертая координата чтобы можно было умножать на матрицу 4х4

    std::vector<GLfloat> normals = { 0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f };

    std::vector<GLfloat> texCoords = { 0.0f, 0.0f,
                                     1.0f, 0.0f,
                                     0.0f, 1.0f,
                                     1.0f, 1.0f };

    std::vector<uint32_t> indices = { 0u, 1u, 2u, 1u, 3u, 2u };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &indicesVBO);
    glBindVertexArray(VAO);


    //position
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    //normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);

    //texCoord
    glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glGenTextures(1, &DiffTexture);
    glBindTexture(GL_TEXTURE_2D, DiffTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, diff_tex.width, diff_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, diff_tex.image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(diff_tex.image);

    std::string mat = "material.diffuseMap1";
    glUniform1i(glGetUniformLocation(shader.Program, mat.c_str()), 1);
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

    mat = "material.specularMap1";
    glUniform1i(glGetUniformLocation(shader.Program, mat.c_str()), 1);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);


    //normalmaps
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

    mat = "material.normalMap1";
    glUniform1i(glGetUniformLocation(shader.Program, mat.c_str()), 1);
    glBindTexture(GL_TEXTURE_2D, BumpTexture);



    
    

    indicSize = indices.size();

    model = beginModel;

}

void Plane::Draw()
{

    shader.setMat4("model", model);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, BumpTexture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicSize, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

}