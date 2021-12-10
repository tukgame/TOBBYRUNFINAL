using namespace std;

GLuint g_window_w = 800;
GLuint g_window_h = 800;

GLuint Hexa_VAO;
GLuint Hexa_VBO[3];

//나무
GLuint Tree_1_VAO;
GLuint Tree_1_VBO[3];
GLuint Tree_2_VAO;
GLuint Tree_2_VBO[3];

//철수
GLuint head_VAO;
GLuint head_VBO[3];
GLuint arm_l_VAO;
GLuint arm_l_VBO[3];
GLuint arm_r_VAO;
GLuint arm_r_VBO[3];
GLuint leg_l_VAO;
GLuint leg_l_VBO[3];
GLuint leg_r_VAO;
GLuint leg_r_VBO[3];

//장애물
GLuint trash_VAO;
GLuint trash_VBO[2];
GLuint oak_VAO;
GLuint oak_VBO[2];

GLuint LINE_VAO;
GLuint LINE_VBO[3];

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
glm::mat4 Rfordie = glm::mat4(1.0f);
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
struct obstacles {
	int show;
	float o_x;
	float o_z;
	int obstacle;
	GLuint VAO;
};

trees t[10];
obstacles obs_l[5];
obstacles obs_c[5];
obstacles obs_r[5];

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
	i = 0;
	for (i; i < 5; i++) {
		obs_l[i].o_x = -1.0f;
		obs_l[i].o_z = -4.0f + (i * 2.0f);
		j = rand() % 2;
		if (j == 0) {
			obs_l[i].obstacle = o;
			obs_l[i].VAO = oak_VAO;
		}
		else {
			obs_l[i].obstacle = tra;
			obs_l[i].VAO = trash_VAO;
		}
		obs_l[i].show = 0;
	}
	i = 0;
	for (i; i < 5; i++) {
		obs_c[i].o_x = 0.0f;
		obs_c[i].o_z = -4.0f + (i * 2.0f);
		j = rand() % 2;
		if (j == 0) {
			obs_c[i].obstacle = o;
			obs_c[i].VAO = oak_VAO;
		}
		else {
			obs_c[i].obstacle = tra;
			obs_c[i].VAO = trash_VAO;
		}
		obs_c[i].show = 0;
	}
	i = 0;
	for (i; i < 5; i++) {
		obs_r[i].o_x = 1.0f;
		obs_r[i].o_z = -4.0f + (i * 2.0f);
		j = rand() % 2;
		if (j == 0) {
			obs_r[i].obstacle = o;
			obs_r[i].VAO = oak_VAO;
		}
		else {
			obs_r[i].obstacle = tra;
			obs_r[i].VAO = trash_VAO;
		}
		obs_r[i].show = 0;
	}
}

float LineData[] = {
	0.5f, 0.0f, -4.0f,
	0.5f, 0.0f, 4.0f,
	-0.5f, 0.0f, -4.0f,
	-0.5f, 0.0f, 4.0f
};
float N[] = {
	0.0f, 1.0f, 0.0f
};

GLuint ShaderID;
//const GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexShader, fragmentShader; // 세이더 객체
GLuint vao, vao2, vao3, vao4, vbo[3], vbo2[3], vbo3[3], vbo4[3];
void Keyboard(unsigned char key, int x, int y);

GLenum mod = GL_TRIANGLES;

GLfloat geoTriTexture[3][3] = {
	{0.5, 1.0}, {0.0, 0.0}, {1.0,0.0}
};

GLfloat BRec[6][3] = { // 사각형 위치 값
	{-1.0,-1.0,0.0},{1.0,-1.0,0.0},{1.0,1.0,0.0},{1.0,1.0,0.0},{-1.0,1.0,0.0},{-1.0,-1.0,0.0}
};
GLfloat Breccol[6][3] = { // 사각형 꼭짓점 색상
{ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } };
GLfloat sixTexture[6][2] = {
	 {0.0,0.0}, {1.0, 0.0}, {1.0, 1.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0,0.0}
};


int click = 0;

const char* filetobuf(const char* file)
{
	FILE* fptr;
	char* buf;
	long length;

	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) { // Return NULL on failure
		return NULL;
	}
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = NULL; // Null terminator
	return buf; // Return the buffer
}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex2.glsl");
	//vertexsource = filetobuf("v1.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		//return false;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment2.glsl");
	//fragmentsource = filetobuf("F1.glfs");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		//return false;
	}
}


void InitBuffermenu() {
	glGenVertexArrays(1, &vao4); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao4);
	glGenBuffers(3, vbo4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), BRec, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4[1]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), Breccol, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sixTexture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}
GLuint s_program2;

GLuint InitShader2()
{
	GLint result;
	GLchar errorLog[512];
	make_vertexShaders();
	make_fragmentShaders();
	//-- shader Program
	s_program2 = glCreateProgram();
	glAttachShader(s_program2, vertexShader);
	glAttachShader(s_program2, fragmentShader);
	glLinkProgram(s_program2);
	glGetProgramiv(s_program2, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program2, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		//return false;
	}
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program2);
	return s_program2;
}

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

unsigned int tex_head, tex_arm_l, tex_arm_r, tex_leg_l, tex_leg_r, texture_2d;
BITMAPINFO* bmp;

void InitTexture()
{
	glGenTextures(1, &tex_head);
	glBindTexture(GL_TEXTURE_2D, tex_head);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	unsigned char* data = stbi_load("headbody_UV_color.bmp", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glGenTextures(1, &texture_2d);
	glBindTexture(GL_TEXTURE_2D, texture_2d);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	//int width, height, nrChannels;
	data = stbi_load("tobby.bmp", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// 텍스처 로드 및 생성
	////int width, height, nrChannels;
	//data = stbi_load("aa.bmp", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// 텍스처 로드 및 생성
	////int width, height, nrChannels;
	//data = stbi_load("sky.bmp", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);
}

void InitBuffer()
{
	//바닥
	glGenVertexArrays(1, &Hexa_VAO);
	glGenBuffers(2, Hexa_VBO);

	hexadragon = h.loadObj_normalize_center("c.obj");

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

	glBindBuffer(GL_ARRAY_BUFFER, Hexa_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, h.outvertex.size() * sizeof(glm::vec2), &h.outuv[0], GL_STATIC_DRAW);
	GLint tAttribute = glGetAttribLocation(s_program, "vTexCoord");
	glVertexAttribPointer(tAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

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
	glGenBuffers(3, head_VBO);

	hb = headbody.loadObj_normalize_center("headbody_f.obj");

	glUseProgram(s_program);
	glBindVertexArray(head_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, head_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, headbody.outvertex.size() * sizeof(glm::vec3), &headbody.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, head_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, headbody.outnormal.size() * sizeof(glm::vec3), &headbody.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, head_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, headbody.outuv.size() * sizeof(glm::vec2), &headbody.outuv[0], GL_STATIC_DRAW);
	tAttribute = glGetAttribLocation(s_program, "vTexCoord");
	glVertexAttribPointer(tAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

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


	glGenVertexArrays(1, &oak_VAO);
	glGenBuffers(2, oak_VBO);

	o = oak.loadObj_normalize_center("oak.obj");

	glUseProgram(s_program);
	glBindVertexArray(oak_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, oak_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, oak.outvertex.size() * sizeof(glm::vec3), &oak.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, oak_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, oak.outvertex.size() * sizeof(glm::vec3), &oak.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glGenVertexArrays(1, &trash_VAO);
	glGenBuffers(2, trash_VBO);

	tra = trash.loadObj_normalize_center("trash.obj");

	glUseProgram(s_program);
	glBindVertexArray(trash_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, trash_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, trash.outvertex.size() * sizeof(glm::vec3), &trash.outvertex[0], GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(s_program, "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, trash_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, trash.outvertex.size() * sizeof(glm::vec3), &trash.outnormal[0], GL_STATIC_DRAW);
	nAttribute = glGetAttribLocation(s_program, "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &LINE_VAO);
	glGenBuffers(2, LINE_VBO);
	glBindVertexArray(LINE_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), LineData, GL_STATIC_DRAW);
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