// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/ext.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "objReader.h"
#include "stb_image.h"
#include "init.h"

using namespace std;

void Display();
void Keyboard(unsigned char key, int x, int y);
void Timefunc(int value);
void Timefunc2(int value);
void Timefunc3(int value);
void Timefunc4(int value);
void TimerJump(int value);
void Timefunc_gameend(int value);
void crush();

int j_value = 0;
float move_y = 0;
int j_d = 0;
float rotate4die = 180.0f;
int n_value = 0;
int m_value = 0;
int heart = 0;
int move_point = 1;
int menu_point = 0;

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
	InitShader2();

	InitTexture();

	InitBuffer();
	InitBuffermenu();
	set();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	//glutTimerFunc(10, Timefunc, 1);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	return 0;
}

float a = 0.0f;
int useshader = 0;

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
	T = glm::translate(model, glm::vec3(0.0f + exmove, -0.11f + move_y, 4.0f));
	R = glm::rotate(model, glm::radians(rotate4die), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R));

	glBindVertexArray(head_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_head);
	glDrawArrays(GL_TRIANGLES, 0, hb);

	S = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.15f));
	T = glm::translate(model, glm::vec3(-0.18f + exmove, -0.31f + move_y, 4.0f));
	R = glm::rotate(model, glm::radians(rotate4die), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(-a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(arm_l_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_arm_l);
	glDrawArrays(GL_TRIANGLES, 0, a_l);

	S = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.15f));
	T = glm::translate(model, glm::vec3(0.18f + exmove, -0.31f + move_y, 4.0f));
	R = glm::rotate(model, glm::radians(rotate4die), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(arm_r_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_arm_r);
	glDrawArrays(GL_TRIANGLES, 0, a_r);

	S = glm::scale(model, glm::vec3(0.2f, 0.25f, 0.23f));
	T = glm::translate(model, glm::vec3(-0.07f + exmove, -0.5f + move_y, 4.0f));
	R = glm::rotate(model, glm::radians(rotate4die), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(-a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(leg_l_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_leg_l);
	glDrawArrays(GL_TRIANGLES, 0, l_l);

	S = glm::scale(model, glm::vec3(0.2f, 0.25f, 0.23f));
	T = glm::translate(model, glm::vec3(0.07f + exmove, -0.5f + move_y, 4.0f));
	R = glm::rotate(model, glm::radians(rotate4die), glm::vec3(0.0f, 1.0f, 0.0f));
	TRS = glm::rotate(model, glm::radians(a), glm::vec3(1.0f, 0.0f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S * R * TRS));

	glBindVertexArray(leg_r_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_leg_r);
	glDrawArrays(GL_TRIANGLES, 0, l_r);


	//바닥
	/*outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 0.6f, 0.3f);*/

	S = glm::scale(model, glm::vec3(5.0f, 3.0f, 7.0f));
	T = glm::translate(model, glm::vec3(0.0f, -3.7f, 0.0f));
	modelLocation = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

	glBindVertexArray(Hexa_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ground);
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

	outColorLocation = glGetUniformLocation(s_program, "Out_Color");
	glUniform3f(outColorLocation, 1.0f, 1.0f, 1.0f);


	for (int i = 0; i < 5; i++) {
		if (obs_l[i].show == 1) {
			S = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			T = glm::translate(model, glm::vec3(obs_l[i].o_x, -0.4f, obs_l[i].o_z));
			modelLocation = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

			glBindVertexArray(obs_l[i].VAO);
			if (obs_l[i].obstacle == o) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_oak);
			}
			else if (obs_l[i].obstacle == tra) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_trash);
			}
			glDrawArrays(GL_TRIANGLES, 0, obs_l[i].obstacle);
		}
	}
	for (int i = 0; i < 5; i++) {
		if (obs_c[i].show == 1) {
			S = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			T = glm::translate(model, glm::vec3(obs_c[i].o_x, -0.4f, obs_c[i].o_z));
			modelLocation = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

			glBindVertexArray(obs_c[i].VAO);
			if (obs_c[i].obstacle == o) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_oak);
			}
			else if (obs_l[i].obstacle == tra) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_trash);
			}
			glDrawArrays(GL_TRIANGLES, 0, obs_c[i].obstacle);
		}
	}
	for (int i = 0; i < 5; i++) {
		if (obs_r[i].show == 1) {
			S = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			T = glm::translate(model, glm::vec3(obs_r[i].o_x, -0.4f, obs_r[i].o_z));
			modelLocation = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T * S));

			glBindVertexArray(obs_r[i].VAO);
			if (obs_r[i].obstacle == o) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_oak);
			}
			else if (obs_l[i].obstacle == tra) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_trash);
			}
			glDrawArrays(GL_TRIANGLES, 0, obs_r[i].obstacle);
		}
	}




	glUseProgram(s_program2);
	if (menu_point == 0) {
		glBindVertexArray(vao4);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_2d);
		glDrawArrays(mod, 0, 9);
	}
	glutSwapBuffers();

}

int roofd_10 = 0;
int roofa_10 = 0;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'g': { //뒤에서보기
		menu_point++;
		glutTimerFunc(10, Timefunc, 1);
		glutTimerFunc(10, Timefunc4, 1);
		glutTimerFunc(1000, Timefunc_gameend, 1);
		break;
	}
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
	case 'B': {
		camera_x = 0.0f;
		camera_y = 0.0f;
		camera_z = 4.0f;
		break;
	}
	case 'U': {
		camera_x = 8.0f;
		camera_y = 0.0f;
		camera_z = 0.0f;
		break;
	}
	case 'a': {
		if (move_point != 0) {
			move_point--;
			roofa_10 = 0;
			//exmove = exmove - 1.0f;
			glutTimerFunc(10, Timefunc3, 1);
		}
		break;
	}
	case 'd': {
		if (move_point != 2) {
			move_point++;
			roofd_10 = 0;
			//exmove = exmove + 1.0f;
			glutTimerFunc(10, Timefunc2, 1);
		}
		break;
	}
	case 'j': {
		j_value = 1;
		glutTimerFunc(50, TimerJump, 1);
		break;
	}
	case 'q': {
		exit(EXIT_FAILURE);
		break;
	}
	}
	glutPostRedisplay();
}


int move_value = 1;
void Timefunc(int value) {
	if (move_value == 0) {
		return;
	}
	else {

		for (int i = 0; i < 10; ++i) {
			if (t[i].t_z >= 4.0f) {
				t[i].t_z = -4.0f;
			}
			t[i].t_z += 0.05f;
		
		}
		for (int i = 0; i < 5; ++i) {
			if (obs_l[i].o_z >= 6.0f) {
				obs_l[i].o_z = -4.0f;
				obs_l[i].show = rand() % 2;
				if (obs_c[i].show == 1 || obs_r[i].show == 1) {
					obs_l[i].show = 0;
				}
			}
			obs_l[i].o_z += 0.05f;
		}
		for (int i = 0; i < 5; ++i) {
			if (obs_c[i].o_z >= 6.0f) {
				obs_c[i].o_z = -4.0f;
				obs_c[i].show = rand() % 2;
				if (obs_l[i].show == 1 || obs_r[i].show == 1) {
					obs_c[i].show = 0;
				}
			}
			obs_c[i].o_z += 0.05f;
		}
		for (int i = 0; i < 5; ++i) {
			if (obs_r[i].o_z >= 6.0f) {
				obs_r[i].o_z = -4.0f;
				obs_r[i].show = rand() % 2;
				if (obs_c[i].show == 1 || obs_l[i].show == 1) {
					obs_r[i].show = 0;
				}
			}
			obs_r[i].o_z += 0.05f;
		}
		crush();
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
	if (move_value != 0) {
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
	}

	glutPostRedisplay();
	glutTimerFunc(1, Timefunc4, 1);
	
}

int shake_time = 0;

void Timefunc5(int value) {
	if (shake_time == 0) {
		camera_x += 0.3f;
		shake_time++;
		glutPostRedisplay();
		glutTimerFunc(10, Timefunc5, 1);
	}
	else if (shake_time == 1) {
		camera_x -= 0.6f;
		shake_time++;
		glutPostRedisplay();
		glutTimerFunc(10, Timefunc5, 1);

	}
	else if (shake_time == 2) {
		camera_x += 0.3f;
		shake_time++;
		glutPostRedisplay();
		glutTimerFunc(10, Timefunc5, 1);

	}
	else if (shake_time == 3) {
		shake_time = 0;
	}

}

int game_time = 0;

void Timefunc_gameend(int value) {
	game_time++;

	sky_r -= 0.01f;
	sky_g -= 0.01f;
	sky_b -= 0.01f;

	light_r -= 0.03f;
	light_g -= 0.03f;
	light_b -= 0.03f;

	if (game_time > 30) {
		
	}

	if (game_time < 30) {
		glutPostRedisplay();
		glutTimerFunc(1000, Timefunc_gameend, 1);
	}
	
}

void crush() {
	for (int i = 0; i < 5; i++) {
		if (obs_l[i].o_z >= 3.95f && obs_l[i].o_z <= 4.05f && obs_l[i].show == 1 && j_d == 0) {
			if (exmove <= -1.0f) {
				if (heart == 1) {
					obs_l[i].o_z -= 0.3f;
					move_value = 0;
					//rotate4die += 180.0f;
					glutPostRedisplay();
					return;
				}
				else {
					if (move_point != 2) {
						move_point++;
						roofd_10 = 0;
						//exmove = exmove + 1.0f;
						glutTimerFunc(1, Timefunc2, 1);
					}
					cout << "heart = 1" << endl;
					glutTimerFunc(10, Timefunc5, 1);
					heart = 1;
					return;
				}
			}
		}
		if (obs_r[i].o_z >= 3.95f && obs_r[i].o_z <= 4.05f && obs_r[i].show == 1 && j_d == 0) {
			if (exmove >= 1.0f) {
				if (heart == 1) {
					obs_r[i].o_z -= 0.3f;
					//rotate4die += 180.0f;
					move_value = 0;
					glutPostRedisplay();
					return;
				}
				else {
					if (move_point != 0) {
						move_point--;
						roofa_10 = 0;
						//exmove = exmove - 1.0f;
						glutTimerFunc(1, Timefunc3, 1);
					}
					cout << "heart = 1" << endl;
					glutTimerFunc(10, Timefunc5, 1);
					heart = 1;
					return;
				}
			}
		}
		if (obs_c[i].o_z >= 3.95f && obs_c[i].o_z <= 4.05f && obs_c[i].show == 1 && j_d == 0) {
			if (exmove >= -0.05f && exmove <= 0.05f) {
				if (heart == 1) {
					obs_c[i].o_z -= 0.3f;
					move_value = 0;
					//rotate4die += 180.0f;
					glutPostRedisplay();
					return;
				}
				else {
					move_point++;
					roofd_10 = 0;
					//exmove = exmove + 1.0f;
					glutTimerFunc(10, Timefunc2, 1);
					cout << "heart = 1" << endl;
					glutTimerFunc(10, Timefunc5, 1);
					heart = 1;
					return;
				}
			}
		}
	}
}
void TimerJump(int value) {
	if (j_value == 0) {
		return;
	}
	else if (j_value == 1) {
			move_y = 0.1 * j_d;
			j_d++;
			if (j_d == 5) {
				j_value = 2;
			}
	}
	else if (j_value == 2) {
			move_y = 0.1 * j_d;
			j_d--;
			if (j_d == 1) {
				j_d = 0;
				j_value = 0;
			}
		
	}
	glutPostRedisplay();
	glutTimerFunc(50, TimerJump, 1);
}