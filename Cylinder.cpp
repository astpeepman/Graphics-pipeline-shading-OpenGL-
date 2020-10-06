#include "Cylinder.h"
#include <vector>

Cylinder::Cylinder(vec3 p, GLfloat r, Shader s, GLfloat h, int numberSlices)
{
    position = p;
    radius = r;
    height = h;
    shader = s;
    beginModel = mat4(1.0f);
    beginModel = glm::translate(beginModel, position);

    int i, j;

    int numberParallels = numberSlices;
    int numberVertices = numberSlices * 2 + 2;
    int numberIndices = (numberSlices * 4) * 3;

    float angleStep = (2.0f * 3.14159265358979323846f) / ((float)numberSlices); //число секторов деления в радианах
    //float helpVector[3] = {0.0f, 1.0f, 0.0f};

    std::vector<GLfloat> vertex(numberVertices * 4, 0.0f);
    std::vector<GLfloat> normals(numberVertices * 4, 0.0f);
    std::vector<GLfloat> texcoords(numberVertices * 2, 0.0f);
    std::vector<int> indices(numberIndices, -1);

    vertex.at(0) = 0.0f;
    vertex.at(1) = 0.0f;
    vertex.at(2) = 0.0f;
    vertex.at(3) = 1.0f;
    vertex.at(4) = 0.0f;
    vertex.at(5) = 1.0f * height;
    vertex.at(6) = 0.0f;
    vertex.at(7) = 1.0f;

    normals.at(0) = 0.0f;
    normals.at(1) = -1.0f;
    normals.at(2) = 0.0f;
    normals.at(3) = 1.0f;
    normals.at(4) = 0.0f;
    normals.at(5) = 1.0f;
    normals.at(6) = 0.0f;
    normals.at(7) = 1.0f;

    for (i = 1; i < numberVertices / 2; i++)
    {
        int vertexIndex = i * 8;
        int normalIndex = i * 8;
        int texCoordsIndex = i * 2;

        vertex.at(vertexIndex + 0) = radius * sinf(angleStep * (GLfloat)i);
        vertex.at(vertexIndex + 1) = 0.0f;
        vertex.at(vertexIndex + 2) = radius * cosf(angleStep * (GLfloat)i);
        vertex.at(vertexIndex + 3) = 1.0f;

        vertex.at(vertexIndex + 4) = radius * sinf(angleStep * (GLfloat)i);
        vertex.at(vertexIndex + 5) = 1.0f * height;
        vertex.at(vertexIndex + 6) = radius * cosf(angleStep * (GLfloat)i);
        vertex.at(vertexIndex + 7) = 1.0f;

        normals.at(normalIndex + 0) = vertex.at(vertexIndex + 0);
        normals.at(normalIndex + 1) = 0.0f;
        normals.at(normalIndex + 2) = vertex.at(vertexIndex + 2);
        normals.at(normalIndex + 3) = 1.0f;

        normals.at(normalIndex + 4) = vertex.at(vertexIndex + 0);
        normals.at(normalIndex + 5) = 0.0f;
        normals.at(normalIndex + 6) = vertex.at(vertexIndex + 2);
        normals.at(normalIndex + 7) = 1.0f;

        //texcoords.at(texCoordsIndex + 0) = (float)j / (float)numberSlices;
        //texcoords.at(texCoordsIndex + 1) = (1.0f - (float)i) / (float)(numberParallels - 1);
    }

    for (i = 0; i < numberIndices / 6; i++)
    {
        int index = i * 6;

        if ((i % 2) == 0)
        {
            indices.at(index) = 0;
            indices.at(index + 1) = (i + 2) % (numberVertices);
            indices.at(index + 2) = (i + 4) % (numberVertices);
            if (((i + 4) % numberVertices) == 0)  indices.at(index + 2) = 2;
            indices.at(index + 3) = indices.at(index + 1);
            indices.at(index + 4) = indices.at(index + 2);
            indices.at(index + 5) = indices.at(index + 1) + 1;
        }
        if ((i % 2) != 0)
        {
            indices.at(index) = 1;
            indices.at(index + 1) = (i + 2) % (numberVertices);
            indices.at(index + 2) = (i + 4) % (numberVertices);
            if (((i + 4) % numberVertices) == 1)  indices.at(index + 2) = 3;
            indices.at(index + 3) = indices.at(index + 1);
            indices.at(index + 4) = indices.at(index + 2);
            indices.at(index + 5) = indices.at(index + 2) - 1;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &indicesVBO);
    glBindVertexArray(VAO);


    //vertexition
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    //texCoord
    /*glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);*/


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);


    shader.Use();
    indicSize = indices.size();

    model = beginModel;
}

void Cylinder::draw()
{
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicSize, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}



void Cylinder::setLightParam(vec3 ambient, vec3 diff, vec3 spec)
{
    shader.setVec3("light.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("light.diffuse", diff.x, diff.y, diff.z);
    shader.setVec3("light.specular", spec.x, spec.y, spec.z);
}

void Cylinder::setLightPos(vec3 p)
{
    shader.setVec3("light.position", p);
}

void Cylinder::setViewPos(vec3 p)
{
    shader.setVec3("viewPos", p);
}

void Cylinder::setproj(mat4 prj)
{
    shader.setMat4("projection", prj);

}

void Cylinder::setview(mat4 view)
{
    shader.setMat4("view", view);
}

void Cylinder::setmaterial(vec3 am, vec3 diff, vec3 spec, GLfloat sh)
{
    shader.setVec3("material.ambient", am.x, am.y, am.z);
    shader.setVec3("material.diffuse", diff.x, diff.y, diff.z);
    shader.setVec3("material.specular", spec.x, spec.y, spec.z); // specular lighting doesn't have full effect on this object's material
    shader.setFloat("material.shininess", sh);
}

void Cylinder::Rotate(GLfloat angle, vec3 axis)
{
    model = glm::rotate(model, radians(angle), axis);
}

void Cylinder::Trans(vec3 pos)
{
    model = glm::translate(model, pos);
}
