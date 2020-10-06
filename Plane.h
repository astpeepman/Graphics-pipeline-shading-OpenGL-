#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dMath.h"
#include "Shader.h"
class Plane
{
private:
	vec3 position;
	Shader shader;
	mat4 model;
	GLuint vertexVBO, normalsVBO, texcoordVBO, indicesVBO;
	GLuint VAO, VBO;
	GLsizei indicSize;
	mat4 beginModel;
	Texture diff_tex;
	Texture spec_tex;
	Texture norm_tex;
	GLuint DiffTexture, SpecTexture, BumpTexture;
public:
	Plane();
	Plane(vec3 pos, Shader s, Texture dt, Texture st, Texture nt);
	void Draw();
};

