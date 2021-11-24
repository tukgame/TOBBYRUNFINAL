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
void Timefunc(int value);

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
	set();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(10, Timefunc, 1);
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
	//조명
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, 0.0f, 5.0f, 2.0f);
	viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, 0.0f, 0.5f, 2.0f);
	lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	glUniform3f(lightColorLocation, light_r, light_g, light_b);



	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.0f, 0.0f);

	////태양
	//S = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	//T = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//modelLocation = glGetUniformLocation(s_program, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	//glBindVertexArray(Hexa_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.6f, 0.3f);

	//바닥
	S = glm::scale(model, glm::vec3(5.0f, 3.0f, 5.0f));
	T = glm::translate(model, glm::vec3(0.0f, -3.7f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hexadragon);	
	
	//나무_1

	
	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 0.0f, 0.8f, 0.0f);
	
	for (int i = 0; i < 10; i++) {
		S = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.7f));
		T = glm::translate(model, glm::vec3(t[i].t_x, 0.3f, t[i].t_z));
		modelLocation = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

		glBindVertexArray(Tree_1_VAO);
		glDrawArrays(GL_TRIANGLES, 0, t[i].tree);
	}

	glutSwapBuffers();

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a': { //낮

		sky_r = 0.4f;
		sky_g = 0.6f;
		sky_b = 0.8f;

		light_r = 1.0f;
		light_g = 1.0f;
		light_b = 1.0f;
		break;
	}
	case 'n': { //밤
		sky_r = 0.0f;
		sky_g = 0.0f;
		sky_b = 0.1f;

		light_r = 0.5f;
		light_g = 0.5f;
		light_b = 0.5f;
		break;
	}
	case 'u': { //위에서보기
		camera_x = 0.0f;
		camera_y = 3.0f;
		camera_z = 0.1f;
		break;
	}
	case 'b': { //뒤에서보기
		camera_x = 0.0f;
		camera_y = 0.7f;
		camera_z = 2.0f;
		break;
	}
	case 'q': {
		exit(EXIT_FAILURE);
		break;
	}
	}
	glutPostRedisplay();
}

void Timefunc(int value) {
	for (int i = 0; i < 10; ++i) {
		if (t[i].t_z >= 4.0f) {
			t[i].t_z = -4.0f;
		}
		t[i].t_z += 0.05f;
		
	}
	glutPostRedisplay();
	glutTimerFunc(10, Timefunc, 1);
}