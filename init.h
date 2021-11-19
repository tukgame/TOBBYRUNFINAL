GLuint g_window_w = 600;
GLuint g_window_h = 600;

GLuint Hexa_VAO;
GLuint Hexa_VBO[2];

GLuint Py_VAO;
GLuint Py_VBO[2];
GLuint Light_VAO;
GLuint Light_VBO[2];

GLuint LINE_VAO;
GLuint LINE_VBO[2];

objReader h;
objReader l;
objReader p;

float light_r = 1.0f;
float light_g = 1.0f;
float light_b = 1.0f;

float z_light = 3.0f;
float y_rotate = 0.0f;
float r_light = 0.0f;
float y_light = 0.0f;

int hexadragon;
int pyramid;
int light;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);
glm::mat4 S = glm::mat4(1.0f);
glm::mat4 R = glm::mat4(1.0f);
glm::mat4 TRS = glm::mat4(1.0f);
unsigned int modelLocation;
int lightPosLocation;
int viewPosLocation;
int lightColorLocation;
int outColorLocation;

float LineData[] = {
	-3.0f, 0.0f, 0.0f,
	3.0f, 0.0f, 0.0f,
	0.0f, -3.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	0.0f, 0.0f, -3.0f,
	0.0f, 0.0f, 3.0f,
};
float N[] = {
	0.0f, 1.0f, 0.0f
};


void InitBuffer()
{

	//// 5.1. VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &Hexa_VAO);
	glGenVertexArrays(1, &Py_VAO);
	glGenVertexArrays(1, &Light_VAO);
	glGenBuffers(2, Hexa_VBO);
	glGenBuffers(2, Py_VBO);
	glGenBuffers(2, Light_VBO);

	hexadragon = h.loadObj_normalize_center("s.obj");

	// 2 triangles for quad floor
	glUseProgram(s_program);
	glBindVertexArray(Hexa_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Hexa_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, h.outvertex.size() * sizeof(glm::vec3), &h.outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Hexa_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, h.outvertex.size() * sizeof(glm::vec3), &h.outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	light = l.loadObj_normalize_center("s.obj");

	// 2 triangles for quad floor
	glUseProgram(s_program);
	glBindVertexArray(Light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Light_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, l.outvertex.size() * sizeof(glm::vec3), &l.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Light_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, l.outvertex.size() * sizeof(glm::vec3), &l.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	pyramid = p.loadObj_normalize_center("s.obj");

	glUseProgram(s_program);
	glBindVertexArray(Py_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Py_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, p.outvertex.size() * sizeof(glm::vec3), &p.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Py_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, p.outvertex.size() * sizeof(glm::vec3), &p.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//// 5.1. VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &LINE_VAO);
	glGenBuffers(2, LINE_VBO);

	glBindVertexArray(LINE_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), LineData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), N, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

}

void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}
void makeCP() {
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 2.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(30.0f), (float)800 / (float)800, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -10.0));
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}