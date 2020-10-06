#include "SphereObject.h"
#include <vector>

SphereObject::SphereObject(vec3 p, GLfloat r, Shader s, Texture dt, Texture st, int numberSlices)
{
	position = p;
	radius = r;
	shader = s;
	diff_tex = dt;
	spec_tex = st;

    beginModel = mat4(1.0f);
    beginModel = glm::translate(beginModel, position);
    beginModel = rotate(beginModel, radians(180.0f), vec3(1, 0, 1));
    int i, j;

    int numberParallels = numberSlices;
    int numberVertices = (numberParallels + 1) * (numberSlices + 1);
    int numberIndices = numberParallels * numberSlices * 3;

    GLfloat angleStep = (2.0f * 3.14159265358979323846f) / ((GLfloat)numberSlices);
    //float helpVector[3] = {0.0f, 1.0f, 0.0f};

    std::vector<float> vertex(numberVertices * 3, 0.0f);
    std::vector<float> normals(numberVertices * 3, 0.0f);
    std::vector<float> texCoords(numberVertices * 2, 0.0f);

    std::vector<int> indices(numberIndices, -1);

    

    for (i = 0; i < numberParallels + 1; i++)
    {
        for (j = 0; j < numberSlices + 1; j++)
        {
            int vertexIndex = (i * (numberSlices + 1) + j) * 3;
            int normalIndex = (i * (numberSlices + 1) + j) * 3;
            int texCoordsIndex = (i * (numberSlices + 1) + j) * 2;

            vertex[vertexIndex + 0]= radius * sinf(angleStep * (GLfloat)i) * sinf(angleStep * (GLfloat)j);
            vertex[vertexIndex + 1] = radius * cosf(angleStep * (GLfloat)i);
            vertex[vertexIndex + 2] = radius * sinf(angleStep * (GLfloat)i) * cosf(angleStep * (GLfloat)j);


            normals[normalIndex + 0] = vertex[vertexIndex + 0] / radius;
            normals[normalIndex + 1] = vertex[vertexIndex + 1] / radius;
            normals[normalIndex + 2] = vertex[vertexIndex + 2] / radius;


            texCoords[texCoordsIndex + 0] = (GLfloat)j / (GLfloat)numberSlices;
            texCoords[texCoordsIndex + 1] = (1.0f - (GLfloat)i*2) / (GLfloat)(numberParallels - 1);
        }
    }

    int* indexBuf = &indices[0];

    for (i = 0; i < numberParallels; i++)
    {
        for (j = 0; j < numberSlices; j++)
        {
            *indexBuf++ = i * (numberSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numberSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numberSlices + 1) + (j + 1);

            *indexBuf++ = i * (numberSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numberSlices + 1) + (j + 1);
            *indexBuf++ = i * (numberSlices + 1) + (j + 1);

            int diff = int(indexBuf - &indices[0]);
            if (diff >= numberIndices)
                break;
        }
        int diff = int(indexBuf - &indices[0]);
        if (diff >= numberIndices)
            break;
    }

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
    indicSize = indices.size();

    model = beginModel;
}

void SphereObject::SetDfSp()
{
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
}

void SphereObject::setLightParam(vec3 ambient, vec3 diff, vec3 spec)
{
    shader.setVec3("light.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("light.diffuse", diff.x, diff.y, diff.z);
    shader.setVec3("light.specular", spec.x, spec.y, spec.z);
}

void SphereObject::setShinises(GLfloat s)
{
    shader.setFloat("material.shininess", s);
}


void SphereObject::Draw()
{
    
    shader.setMat4("model", model);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, SpecTexture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicSize, GL_UNSIGNED_INT, nullptr);
   
    glBindVertexArray(0); 

}

void SphereObject::setDiff(const std::string& name)
{
    shader.setInt(name, 0);
}

void SphereObject::setLightPos(vec3 p)
{
    shader.setVec3("light.position", p);
}

void SphereObject::setViewPos(vec3 p)
{
    shader.setVec3("viewPos", p);
}

void SphereObject::setproj(mat4 prj)
{
    shader.setMat4("projection", prj);

}

void SphereObject::setview(mat4 view)
{
    shader.setMat4("view", view);
}

void SphereObject::Rotate(GLfloat angle, vec3 axis)
{
    model = glm::rotate(model,radians(angle), axis);
}

void SphereObject::Trans(vec3 pos)
{
    model = glm::translate(model, pos);
}

void SphereObject::Planet(vec3 p, GLfloat speed, vec3 axis)
{
    model = translate(beginModel, p);
    model = rotate(model,radians(speed), axis);
}

void SphereObject::Sputnik(vec3 p, GLfloat speed, vec3 axis, mat4 PlanetPos)
{
    
    model = translate(PlanetPos, p);
    model = rotate(model, radians(speed), axis);
}


