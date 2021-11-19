// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glm/glm.hpp"
#include "GL/glm/ext.hpp"
#include "GL/glm/gtc/matrix_transform.hpp"
#include "shader25.h"
#include "objRead.h"
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(s_program);

	makeCP();

	//조명
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, 3.0f * sin(glm::radians(r_light)), 0.0f, 3.0f * cos(glm::radians(r_light)));
	viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, 0.0f, 0.5f, 2.0f);
	lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	glUniform3f(lightColorLocation, light_r, light_g, light_b);



	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.0f, 0.0f);

	//태양
	S = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	T = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	//지구
	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 0.0f, 1.0f, 0.0f);

	S = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	T = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	//달
	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 0.0f, 0.0f, 1.0f);

	S = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	T = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	glutSwapBuffers();

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': {
		light_r = 1.0f;
		light_g = 0.1f;
		light_b = 0.1f;
		break;
	}
	case 'g': {
		light_r = 0.1f;
		light_g = 1.0f;
		light_b = 0.1f;
		break;
	}
	case 'b': {
		light_r = 0.1f;
		light_g = 0.1f;
		light_b = 1.0f;
		break;
	}
	case 'c': {
		light_r = 1.0f;
		light_g = 1.0f;
		light_b = 1.0f;
		break;
	}
	case 'y': {
		r_light += 5.0f;
		break;
	}
	case 'Y': {
		r_light += 5.0f;
		break;
	}
	case 'q': {
		exit(EXIT_FAILURE);
		break;
	}
	}
	glutPostRedisplay();
}