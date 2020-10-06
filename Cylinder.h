#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dMath.h"
#include "Shader.h"
class Cylinder
{
private:
	vec3 position;
	GLfloat radius;
	GLfloat height;
	Shader shader;
	mat4 model;
	GLuint vertexVBO, normalsVBO, texcoordVBO, indicesVBO;
	GLuint VAO, VBO;
	GLsizei indicSize;
	mat4 beginModel;
public:
	Cylinder() {};
	Cylinder(vec3 p, GLfloat r, Shader s, GLfloat h, int numberSlices);
	void draw();
	void setLightParam(vec3 ambient, vec3 diff, vec3 spec);
	void setLightPos(vec3 p);
	void setViewPos(vec3 p);
	void setproj(mat4 prj);
	void setview(mat4 view);
	void setmaterial(vec3 am, vec3 diff, vec3 spec, GLfloat sh);
	void Rotate(GLfloat angle, vec3 axis);
	void Trans(vec3 pos);
};

