#version 330 core

in vec3 FragPos; //--- 위치값
in vec3 Normal; //--- 버텍스 세이더에서 받은 노멀값
in vec3 lightDir;

out vec4 FragColor; //--- 색상 출력

uniform vec3 Out_Color;
uniform vec3 lightColor; //--- 조명의 색
uniform vec3 viewPos;

void main(void) 
{
   float ambientLight = 0.3;
   vec3 ambient = ambientLight * lightColor;

   vec3 norm = normalize(Normal);
   vec3 light = normalize(lightDir);
   float diffuseLight = max(dot(Normal, light), 0.0);
   float d = 2.0f;
   vec3 diffuse = d * diffuseLight * lightColor;

   int shininess = 128;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-light, Normal);
   float specularLight = max(dot(viewDir, reflectDir), 0.0);
   specularLight = pow(specularLight, shininess);
   vec3 specular = specularLight * lightColor;

   vec3 result = (ambient + diffuse + specular) * Out_Color;

   FragColor = vec4(result, 1.0f);
}