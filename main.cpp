// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/ext.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "objReader.h"
#include "init.h"
using namespace std;

void Display();
void Keyboard(unsigned char key, int x, int y);

int n_value = 0;
int m_value = 0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Computer Graphics");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}
	//add
	Initshader();
	InitBuffer();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	return 0;
}

void Display()
{
	glClearColor(sky_r, sky_g, sky_b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(s_program);

	makeCP();
	//Á¶¸í
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, 0.0f, 5.0f, 2.0f);
	viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, 0.0f, 0.5f, 2.0f);
	lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	glUniform3f(lightColorLocation, light_r, light_g, light_b);



	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.0f, 0.0f);

	//ÅÂ¾ç
	S = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	T = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.6f, 0.3f);

	//¹Ù´Ú
	S = glm::scale(model, glm::vec3(5.0f, 3.0f, 5.0f));
	T = glm::translate(model, glm::vec3(0.0f, -3.7f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	glutSwapBuffers();

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a': { //³·

		sky_r = 0.4f;
		sky_g = 0.6f;
		sky_b = 0.8f;

		light_r = 1.0f;
		light_g = 1.0f;
		light_b = 1.0f;
		break;
	}
	case 'n': { //¹ã
		sky_r = 0.0f;
		sky_g = 0.0f;
		sky_b = 0.1f;

		light_r = 0.5f;
		light_g = 0.5f;
		light_b = 0.5f;
		break;
	}
	case 'q': {
		exit(EXIT_FAILURE);
		break;
	}
	}
	glutPostRedisplay();
}