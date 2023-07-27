#version 330 core

uniform samplerCube skybox;

out vec4 FragColor;
in vec3 texCoord;

void main()
{
	FragColor = texture(skybox, texCoord);
}