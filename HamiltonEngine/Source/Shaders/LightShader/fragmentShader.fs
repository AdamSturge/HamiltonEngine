#version 460 core
out vec4 FragColor;

// texture samplers - take the texture UNIT/channel as an int when passing in data
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform float mixRatio;

// Light
uniform vec3 LightColor;

void main()
{
	FragColor = vec4(LightColor, 1.0);
} 