#ifndef CUBE_H
#define	CUBE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Color.h"
#include "3dMath.h"
#include "Shader.h"
class Cube
{
protected:
	vec3 position;
	GLfloat width, height, depth;
	Texture tex;
	Shader shader;
	vec3 color;
	mat4 model;
	GLuint VAO;
	GLuint texture;
	mat4 projection;
	mat4 view;
	
public:
	Cube();
	Cube(vec3 pos, GLfloat width, GLfloat height, GLfloat depth, Texture t, Shader s, GLuint VBO );
	~Cube();
	bool rotateble;
	void UseShader();
	GLuint ShaderProgram();
	void draw();
	void Rotate(GLfloat angle, vec3 axis);
	void translate(vec3 cubepos);
	void CubeScale(vec3 sc);
};

#endif