#version 460 core
out vec4 FragColor;

// texture samplers - take the texture UNIT/channel as an int when passing in data
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float mixRatio;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = ambient * objectColor;
	FragColor = vec4(result, 1.0);
} 