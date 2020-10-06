#pragma once
#include "Model.h"
class InstanModel :
	public Model
{
private:
	GLuint amount;
	glm::mat4* modelMatrices;
    mat4* buffModels;
	GLfloat InstanRadius;
	GLfloat offset;
    bool gammaCorrection;
    vec3* displacement;
    float* scaleParams;
    vec3* posParams;
public:
    InstanModel() {};

    InstanModel(string const& path, vec3 pos, Shader s, GLuint am, GLfloat r, GLfloat off, GLfloat downScale, GLfloat upScale,  bool gamma = false) : gammaCorrection(gamma)
    {
        model = mat4(1.0f);
        shader = s;

        loadModel(path);

        model = glm::translate(model, pos);

        amount = am;
        modelMatrices = new mat4[amount];
        buffModels = new mat4[amount];
        scaleParams = new float[amount];
        displacement = new vec3[amount];

        srand(glfwGetTime()); // задаем seed для генератора случ. чисел
        InstanRadius = r;
        offset = off;

        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model(1.0f);
            
            //float angle = (float)i / (float)amount * 360.0f;
            displacement[i].x = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            //float x = sin(angle) * InstanRadius + displacement;
            displacement[i].y = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            // высоту поля держим заметно меньшей, чем размеры в плоскости XZ
            //float y = displacement * 0.4f;
            displacement[i].z = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            //float z = cos(angle) * InstanRadius + displacement;
            //posParams[i] = vec3(x, y, z);

            //model = glm::translate(model, glm::vec3(x, y, z));

            int upsc = upScale * 100;

            //масштабирование: случайное масштабирование в пределах (downScale, upScale)
            float scale = (rand() % upsc) / 100.0f + downScale;
            scaleParams[i] = scale;
            model = glm::scale(model, glm::vec3(scale));


            // 4. добавляем в массив матриц
            modelMatrices[i] = model;
            buffModels[i] = model;
        }

        

        

        //unsigned int buffer;
        //glGenBuffers(1, &buffer);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        //for (unsigned int i = 0; i < meshes.size(); i++)
        //{
        //    unsigned int VAO = meshes[i].VAO;
        //    glBindVertexArray(VAO);
        //    // set attribute pointers for matrix (4 times vec4)
        //    glEnableVertexAttribArray(3);
        //    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        //    glEnableVertexAttribArray(4);
        //    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(5);
        //    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(6);
        //    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        //    glVertexAttribDivisor(3, 1);
        //    glVertexAttribDivisor(4, 1);
        //    glVertexAttribDivisor(5, 1);
        //    glVertexAttribDivisor(6, 1);

        //    glBindVertexArray(0);
        //}
    }

    void Draw(mat4 view, mat4 proj);
    void move(float scale);
    
};

