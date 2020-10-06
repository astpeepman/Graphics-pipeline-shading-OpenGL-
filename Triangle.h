#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dMath.h"
#include "Shader.h"




class Triangle
{
private:
	vec3 position;
	vertexNoTex Vectices[3];


public:
	Triangle();
	Triangle(vec3 pos, GLfloat height, GLfloat width);
	~Triangle();

	void draw(Shader s);
	

};



#endif