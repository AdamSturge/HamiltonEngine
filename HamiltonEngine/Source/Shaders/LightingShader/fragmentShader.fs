#version 460 core
out vec4 FragColor;

struct Material {
	// vec3 ambient;
	vec3 diffuseColor;
	vec3 specular;
	float shininess;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	float emissiomPower;
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
in vec2 TexCoords;

// Lighting
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int specularPower;



void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuseMap, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuseMap, TexCoords).rgb;
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // LightDir starts pointing to source, we want to it point away from the source
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular *  spec * texture(material.specularMap, TexCoords).rgb;

	// emission
	vec3 emission = vec3(0.0);
	if (texture(material.specularMap, TexCoords).r == 0.0)
	{
		emission = texture(material.emissionMap, TexCoords).rgb;
	}
	

	vec3 result = (ambient + diffuse + specular) + emission;
	//vec3 result =  norm;
	FragColor = vec4(result, 1.0);
} 