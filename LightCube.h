#pragma once
#include "Cube.h"
class LightCube 
{
	vec3 position;
	Shader shader;
	mat4 model;
	GLuint VAO,VBO;
public:
	LightCube() {};
	LightCube(vec3 pos, Shader s);
	void CubeScale(vec3 sc);
	void Draw();
	void Trans(vec3 pos);
	void UpdateModel();
	void ScaleModel(vec3 scale);
};

