#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Color.h"
#include "3dMath.h"
#include "Shader.h"

class SphereObject
{
private:
	vec3 position;
	GLfloat radius;
	Texture diff_tex;
	Texture spec_tex;
	Shader shader;
	mat4 model;
	GLuint vertexVBO, normalsVBO, texcoordVBO, indicesVBO;
	GLuint VAO, VBO;
	GLuint DiffTexture, SpecTexture;
	GLsizei indicSize;
	mat4 beginModel;
public:
	SphereObject() {};
	SphereObject(vec3 p, GLfloat r, Shader s, Texture dt, Texture st, int numberSlices);
	void SetDfSp();
	void setLightParam(vec3 ambient, vec3 diff, vec3 spec);
	void setShinises(GLfloat s);
	void Draw();
	void setDiff(const std::string& name);
	void setLightPos(vec3 p);
	void setViewPos(vec3 p);
	void setproj(mat4 prj);
	void setview(mat4 view);
	void Rotate(GLfloat angle, vec3 axis);
	void Trans(vec3 pos);
	void Planet(vec3 p, GLfloat speed, vec3 axis);
	void Sputnik(vec3 p, GLfloat speed, vec3 axis, mat4 PlanetPos);
	mat4 GetModel() { return model; };
};

