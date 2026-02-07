#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace HamiltonEngine::OpenGL
{
	Texture::Texture(const char* path, unsigned int internalformat, unsigned int format, unsigned int type)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		int width = 0, height = 0 , nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0); // unbind after creation; technically this is considered an antipattern
	}
	
	void Texture::setTextureOption(unsigned int texOption, unsigned int setting)
	{
		glTexParameteri(GL_TEXTURE_2D, texOption, setting);
	}
	void Texture::setActive()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
}