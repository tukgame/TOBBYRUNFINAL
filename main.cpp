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
void Timefunc2(int value);
void Timefunc3(int value);
void Timefunc4(int value);

int n_value = 0;
int m_value = 0;

int move_point = 1;

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

float a = 0.0f;

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
	glUniform3f(outColorLocation, 1.0f, 1.0f, 1.0f);

	////태양
	//S = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	//T = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//modelLocation = glGetUniformLocation(s_program, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	//glBindVertexArray(Hexa_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, hexadragon);

	//플레이어 몸통
	S = glm::scale(model, glm::vec3(0.4f, 0.45f, 0.5f));
	T = glm::translate(model, glm::vec3(0.0f + exmove, -0.11f, 4.0f));
	R = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R));

	glBindVertexArray(head_VAO);
	glDrawArrays(GL_TRIANGLES, 0, hb);

	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.0f, 0.0f);

	S = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.15f));
	T = glm::translate(model, glm::vec3(-0.18f + exmove, -0.31f, 4.0f));
	R = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(-a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(arm_l_VAO);
	glDrawArrays(GL_TRIANGLES, 0, a_l);

	S = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.15f));
	T = glm::translate(model, glm::vec3(0.18f + exmove, -0.31f, 4.0f));
	R = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(arm_r_VAO);
	glDrawArrays(GL_TRIANGLES, 0, a_r);

	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 0.0f, 0.0f, 1.0f);

	S = glm::scale(model, glm::vec3(0.2f, 0.25f, 0.23f));
	T = glm::translate(model, glm::vec3(-0.07f + exmove, -0.5f, 4.0f));
	R = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(-a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(leg_l_VAO);
	glDrawArrays(GL_TRIANGLES, 0, l_l);

	S = glm::scale(model, glm::vec3(0.2f, 0.25f, 0.23f));
	T = glm::translate(model, glm::vec3(0.07f + exmove, -0.5f, 4.0f));
	R = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(leg_r_VAO);
	glDrawArrays(GL_TRIANGLES, 0, l_r);


	//바닥
	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.6f, 0.3f);

	S = glm::scale(model, glm::vec3(5.0f, 3.0f, 7.0f));
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

		glBindVertexArray(t[i].VAO);
		glDrawArrays(GL_TRIANGLES, 0, t[i].tree);
	}

	glutSwapBuffers();

}

int roofd_10 = 0;
int roofa_10 = 0;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1': { //낮
		sky_r = 0.4f;
		sky_g = 0.6f;
		sky_b = 0.8f;

		light_r = 1.0f;
		light_g = 1.0f;
		light_b = 1.0f;
		break;
	}
	case '2': { //밤
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
		camera_y = 7.0f;
		camera_z = 0.1f;
		break;
	}
	case 'b': { //뒤에서보기
		camera_x = 0.0f;
		camera_y = 0.7f;
		camera_z = 2.0f;
		break;
	}
	case 'B': { //뒤에서보기
		camera_x = 0.0f;
		camera_y = 0.0f;
		camera_z = 4.0f;
		break;
	}
	case 'U': { //뒤에서보기
		camera_x = 8.0f;
		camera_y = 0.0f;
		camera_z = 0.0f;
		break;
	}
	case 'a': { //뒤에서보기
		if (move_point != 0) {
			move_point--;
			roofa_10 = 0;
			//exmove = exmove - 1.0f;
			glutTimerFunc(10, Timefunc3, 1);
		}
		break;
	}
	case 'd': { //뒤에서보기
		if (move_point != 2) {
			move_point++;
			roofd_10 = 0;
			//exmove = exmove + 1.0f;
			glutTimerFunc(10, Timefunc2, 1);
		}
		break;
	}
	case '+': { //뒤에서보기
		glutTimerFunc(10, Timefunc4, 1);
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

void Timefunc2(int value) {
	if (roofd_10 == 10) {

	}
	else {
		roofd_10++;
		exmove += 0.1f;
		glutPostRedisplay();
		glutTimerFunc(1, Timefunc2, 1);
	}
}

void Timefunc3(int value) {
	if (roofa_10 == 10) {

	}
	else {
		roofa_10++;
		exmove -= 0.1f;
		glutPostRedisplay();
		glutTimerFunc(1, Timefunc3, 1);
	}
}

int leg_r_ck = 0;

void Timefunc4(int value) {
	if (leg_r_ck == 0) {
		a -= 0.1f;
	}
	else {
		a += 0.1f;
	}

	if (a < -15.0f) {
		leg_r_ck = 1;
	}
	else if (a > 15.0f) {
		leg_r_ck = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(1, Timefunc4, 1);
	
}