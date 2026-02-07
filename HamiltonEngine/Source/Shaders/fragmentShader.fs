out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers - take the texture UNIT/channel as an int when passing in data
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixRatio;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * -1.0, TexCoord.y)), mixRatio * texture(texture2, TexCoord).a);
}