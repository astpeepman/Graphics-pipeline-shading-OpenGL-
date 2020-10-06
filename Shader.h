#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>; // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL
#include <glm.hpp>

class Shader
{
public:
    // Идентификатор программы
    GLuint Program;
    Shader() {};
    // Конструктор считывает и собирает шейдер
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // Использование программы
    void Use();
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
    }

    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z);
    }

    //
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif