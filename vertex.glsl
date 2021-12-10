#version 330 core

layout (location = 0) in vec3 aPos;   // 위치 변수는 attribute position 0을 가집니다.
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 lightDir;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f); //--- 좌표값에 modelTransform 변환을 적용한다.
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal =  vec3(model * vec4(aNormal, 1.0));
	lightDir = normalize(lightPos - FragPos);
	TexCoord = vTexCoord;

} 