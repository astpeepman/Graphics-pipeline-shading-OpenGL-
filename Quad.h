#pragma once
#ifndef QUAD_H
#define	QUAD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Color.h"
#include "3dMath.h"
#include "Shader.h"

class Quad
{
private:
	vec3 position;
	vertex vertices[4];
	Texture diff_tex;
	Texture spec_tex;
	Texture norm_tex;
	Shader shader;
	GLuint texture;
	mat4 model;
	GLuint vertexVBO, normalsVBO, texcoordVBO, indicesVBO;
	GLuint VAO, VBO;
	GLuint DiffTexture, SpecTexture, BumpTexture;
	GLsizei indicSize;
	bool nrmFlg;
public:
	
	Quad();
	Quad(vec3 pos, GLfloat height, GLfloat width, material mtl, Shader s);

	
	void draw();
	void Rotate(GLfloat angle, vec3 axis);
	void translate(vec3 cubepos);
};

#endif
