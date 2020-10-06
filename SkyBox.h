#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dMath.h"
#include "Shader.h"
class SkyBox
{
private:
	Shader shader;
	GLuint VAO, VBO;
	GLuint SkyTexture;
public:
	SkyBox() {};
	SkyBox(Shader s);
	void draw(mat4 view, mat4 proj, mat4 viewMatrix);
};

