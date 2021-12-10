GLuint g_window_w = 800;
GLuint g_window_h = 800;

GLuint Hexa_VAO;
GLuint Hexa_VBO[2];

//나무
GLuint Tree_1_VAO;
GLuint Tree_1_VBO[2];
GLuint Tree_2_VAO;
GLuint Tree_2_VBO[2];

//철수
GLuint head_VAO;
GLuint head_VBO[2];
GLuint arm_l_VAO;
GLuint arm_l_VBO[2];
GLuint arm_r_VAO;
GLuint arm_r_VBO[2];
GLuint leg_l_VAO;
GLuint leg_l_VBO[2];
GLuint leg_r_VAO;
GLuint leg_r_VBO[2];

//장애물
GLuint trash_VAO;
GLuint trash_VBO[2];
GLuint oak_VAO;
GLuint oak_VBO[2];

GLuint LINE_VAO;
GLuint LINE_VBO[2];

objReader h;
objReader t_1;
objReader t_2;
objReader headbody;
objReader arm_l;
objReader arm_r;
objReader leg_l;
objReader leg_r;
objReader trash;
objReader oak;



float light_r = 1.0f;
float light_g = 1.0f;
float light_b = 1.0f;

float sky_r = 0.4f;
float sky_g = 0.6f;
float sky_b = 0.8f;

float r_light = 0.0f;

float camera_x = 0.0f;
float camera_y = 0.7f;
float camera_z = 2.0f;

int hexadragon;
int tree_1;
int tree_2;
int hb;
int a_l;
int a_r;
int l_l;
int l_r;
int tra;
int o;

float exmove = 0.0f;

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

struct trees {
	float t_x;
	float t_z;
	int tree;
	GLuint VAO;
};

trees t[10];

void set() {
	int i = 0;
	int j = 0;
	srand((unsigned int)time(NULL));

	for (i; i < 5; i++) {
		t[i].t_x = -2.5f;
		t[i].t_z = -4.0f + (i * 2.0f);
		j = rand() % 2;
		if (j == 0) {
			t[i].tree = tree_1;
			t[i].VAO = Tree_1_VAO;
		}
		else {
			t[i].tree = tree_2;
			t[i].VAO = Tree_2_VAO;
		}
	}
	for (i; i < 10; i++) {
		t[i].t_x = 2.5f;
		t[i].t_z = -4.0f + ((i - 5) * 2.0f);
		j = rand() % 2;
		if (j == 1) {
			t[i].tree = tree_1;
			t[i].VAO = Tree_1_VAO;
		}
		else {
			t[i].tree = tree_2;
			t[i].VAO = Tree_2_VAO;
		}
	}
}

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

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	//--- 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	//--- 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	//--- 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	//--- BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	//--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp);
		return NULL;
	}
	//--- 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	//--- 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * ((*info)->bmiHeader.biBitCount + 7.0) / 8.0 * abs((*info)->bmiHeader.biHeight));
	//--- 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);

		return NULL;
	}
	//--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);

		return NULL;
	}
	fclose(fp);

	return bits;
}



void InitBuffer()
{
	//바닥
	glGenVertexArrays(1, &Hexa_VAO);
	glGenBuffers(2, Hexa_VBO);

	hexadragon = h.loadObj_normalize_center("a.obj");

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

	//나무_1
	glGenVertexArrays(1, &Tree_1_VAO);
	glGenBuffers(2, Tree_1_VBO);

	tree_1 = t_1.loadObj_normalize_center("tree_1.obj");

	glUseProgram(s_program);
	glBindVertexArray(Tree_1_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Tree_1_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, t_1.outvertex.size() * sizeof(glm::vec3), &t_1.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Tree_1_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, t_1.outvertex.size() * sizeof(glm::vec3), &t_1.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	
	//나무_2
	glGenVertexArrays(1, &Tree_2_VAO);
	glGenBuffers(2, Tree_2_VBO);

	tree_2 = t_2.loadObj_normalize_center("tree_2.obj");

	glUseProgram(s_program);
	glBindVertexArray(Tree_2_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Tree_2_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, t_2.outvertex.size() * sizeof(glm::vec3), &t_2.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Tree_2_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, t_2.outvertex.size() * sizeof(glm::vec3), &t_2.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);


	//플레이어
	//몸
	glGenVertexArrays(1, &head_VAO);
	glGenBuffers(2, head_VBO);

	hb = headbody.loadObj_normalize_center("headbody_f.obj");

	glUseProgram(s_program);
	glBindVertexArray(head_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, head_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, headbody.outvertex.size() * sizeof(glm::vec3), &headbody.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, head_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, headbody.outvertex.size() * sizeof(glm::vec3), &headbody.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//팔_왼
	glGenVertexArrays(1, &arm_l_VAO);
	glGenBuffers(2, arm_l_VBO);

	a_l = arm_l.loadObj_normalize_center("arm_l_f.obj");

	glUseProgram(s_program);

	glBindVertexArray(arm_l_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, arm_l_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, arm_l.outvertex.size() * sizeof(glm::vec3), &arm_l.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, arm_l_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, arm_l.outvertex.size() * sizeof(glm::vec3), &arm_l.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//팔_오
	glGenVertexArrays(1, &arm_r_VAO);
	glGenBuffers(2, arm_r_VBO);

	a_r = arm_r.loadObj_normalize_center("arm_r_f.obj");

	glUseProgram(s_program);

	glBindVertexArray(arm_r_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, arm_r_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, arm_r.outvertex.size() * sizeof(glm::vec3), &arm_r.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, arm_r_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, arm_r.outvertex.size() * sizeof(glm::vec3), &arm_r.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//다리 왼
	glGenVertexArrays(1, &leg_l_VAO);
	glGenBuffers(2, leg_l_VBO);

	l_l = leg_l.loadObj_normalize_center("leg_l_f.obj");

	glUseProgram(s_program);
	glBindVertexArray(leg_l_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, leg_l_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, leg_l.outvertex.size() * sizeof(glm::vec3), &leg_l.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, leg_l_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, leg_l.outvertex.size() * sizeof(glm::vec3), &leg_l.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//다리 오
	glGenVertexArrays(1, &leg_r_VAO);
	glGenBuffers(2, leg_r_VBO);

	l_r = leg_r.loadObj_normalize_center("leg_r_f.obj");

	glUseProgram(s_program);
	glBindVertexArray(leg_r_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, leg_r_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, leg_r.outvertex.size() * sizeof(glm::vec3), &leg_r.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, leg_r_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, leg_r.outvertex.size() * sizeof(glm::vec3), &leg_r.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//// 5.1. VAO 객체 생성 및 바인딩
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
	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(30.0f), (float)800 / (float)800, 0.1f, 70.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -10.0));
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}