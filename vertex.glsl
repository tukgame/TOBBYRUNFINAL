#version 330 core

layout (location = 0) in vec3 aPos;   // ��ġ ������ attribute position 0�� �����ϴ�.
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;


out vec3 lightDir;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal =  aNormal;
	lightDir = normalize(lightPos - FragPos);
} 