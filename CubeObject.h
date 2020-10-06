#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Color.h"
#include "3dMath.h"
#include "Shader.h"
class CubeObject
{
private:
	vec3 position;
	Texture diff_tex;
	Texture spec_tex;
	Shader shader;
	mat4 model;
	GLuint vertexVBO, normalsVBO, texcoordVBO;
	GLuint VAO, VBO;
	GLuint DiffTexture, SpecTexture;
public:
	CubeObject() {};
	CubeObject(vec3 p, Shader s, Texture dt, Texture st);
	void translate(vec3 tr);
	void rotation(GLfloat angle,vec3 axis);
	void scale(vec3 sc);
	void draw();
	void UseShader();
	GLuint ShaderProgram();
	
};

