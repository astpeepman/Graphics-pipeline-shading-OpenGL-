#include "Triangle.h"


Triangle::Triangle() {

}
Triangle::Triangle(vec3 pos, GLfloat height, GLfloat width) {
	position = pos;
	Vectices[0] = vertexNoTex(vec3(pos.x - width / 2, pos.y, pos.z), vec3(1,0,0));
	Vectices[1] = vertexNoTex(vec3(pos.x, pos.y + height, pos.z), vec3(0, 1, 0));
	Vectices[2] = vertexNoTex(vec3(pos.x+width/2, pos.y, pos.z ), vec3(0, 0, 1));
	
}

Triangle::~Triangle()
{
	glBindVertexArray(0);
}



void Triangle::draw(Shader s) {
	s.Use();
	GLuint TriangleVBO, TriangleVAO;


	glGenVertexArrays(1, &TriangleVAO);
	glGenBuffers(1, &TriangleVBO);

	//Привязываем TriangleVAO
	glBindVertexArray(TriangleVAO);

	//Копируем наш массив вершин в буфер для OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vectices), Vectices, GL_STATIC_DRAW);

	//Устанавливаем указатели на вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered TriangleVBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(TriangleVAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);



}