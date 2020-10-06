#include "InstanModel.h"

void InstanModel::Draw(mat4 view, mat4 proj)
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int VAO = meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    shader.Use();

    shader.setMat4("projection", proj);
    shader.setMat4("view", view);

    shader.setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures_loaded[0].id);
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        glBindVertexArray(meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}

void InstanModel::move(float scale)
{
    
    for (int i = 0; i < amount; i++) {
        float angle = (float)i / (float)amount * 360.0f;
        float x = (sin(angle+scale) * InstanRadius)/scaleParams[i] + displacement[i].x / scaleParams[i];
        float z = (cos(angle + scale) * InstanRadius) / scaleParams[i] + displacement[i].z / scaleParams[i];
        float y = (displacement[i].y * 0.4f) / scaleParams[i];

        modelMatrices[i] = translate(buffModels[i], vec3(x, y, z));
    }
}



