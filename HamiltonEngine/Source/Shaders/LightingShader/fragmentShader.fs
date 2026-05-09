#version 460 core
out vec4 FragColor;

// texture samplers - take the texture UNIT/channel as an int when passing in data
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float mixRatio;

// Lighting
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int specularPower;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	float specularStrenght = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // LightDir starts pointing to source, we want to it point away from the source
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrenght * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	//vec3 result =  norm;
	FragColor = vec4(result, 1.0);
} 