#version 460 core
out vec4 FragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

// texture samplers - take the texture UNIT/channel as an int when passing in data
uniform vec3 objectColor;
uniform float mixRatio;

// Lighting
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int specularPower;

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;


	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // LightDir starts pointing to source, we want to it point away from the source
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * ( spec * material.specular);


	vec3 result = (ambient + diffuse + specular);
	//vec3 result =  norm;
	FragColor = vec4(result, 1.0);
} 