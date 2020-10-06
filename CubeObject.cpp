#include "CubeObject.h"

CubeObject::CubeObject(vec3 p, Shader s, Texture dt, Texture st)
{
	position = p;
	shader = s;
	diff_tex = dt;
    spec_tex = st;
    model = mat4(1.0f);
    translate(position);

    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };
    GLfloat normals[] = {
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,

        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,

        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,

        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f
    };
    GLfloat texCoords[] = {
         0.0f,  0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  0.0f,

         0.0f,  0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  0.0f,

         1.0f,  0.0f,
         1.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  0.0f,
         1.0f,  0.0f,

         1.0f,  0.0f,
         1.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  1.0f,
         0.0f,  0.0f,
         1.0f,  0.0f,

         0.0f,  1.0f,
         1.0f,  1.0f,
         1.0f,  0.0f,
         1.0f,  0.0f,
         0.0f,  0.0f,
         0.0f,  1.0f,

         0.0f,  1.0f,
         1.0f,  1.0f,
         1.0f,  0.0f,
         1.0f,  0.0f,
         0.0f,  0.0f,
         0.0f,  1.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    //position
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    //texCoord
    glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);

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

    

    //текстурцы в шейдер
    shader.Use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    // light properties
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    shader.setFloat("material.shininess", 64.0f);
}

void CubeObject::translate(vec3 tr)
{
    model = glm::translate(model, tr);
}

void CubeObject::rotation(GLfloat angle, vec3 axis)
{
    model= glm::rotate(model, (GLfloat)radians(angle), axis);
}

void CubeObject::scale(vec3 sc)
{
    model = glm::scale(model, sc);
}

void CubeObject::draw()
{
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void CubeObject::UseShader()
{
    shader.Use();
}

GLuint CubeObject::ShaderProgram()
{
    return shader.Program;
}
