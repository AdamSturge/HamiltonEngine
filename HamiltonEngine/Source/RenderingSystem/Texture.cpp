#include "PrecompiledHeader/Pch.h"
#include "Texture.h"
#include "Configuration/Globals.h"
#include <Logging/Logging.h>

#include <stb_image.h>

namespace HamiltonEngine::RenderingSystem
{	
	entt::entity CreateTexture(std::string Path)
	{
		return CreateTexture(Path, DEFAULT_TEXTURE_PARAMS);
	}

	entt::entity CreateTexture(std::string Path, std::map<GLuint, GLuint> ParamsMap)
	{
		entt::registry& Reg = HamiltonEngine::Globals::Registry;
		auto ent = Reg.create();

		TextureIDComponent TID;
		TextureParamatersComponent TPC;
		TPC.Path = Path;
		TPC.TextureParamatersMap = ParamsMap;

		GLuint TexID;

		glGenTextures(1, &TexID);
		
		TID.ID = TexID;

		int Width = 0, Height = 0, NRComponents;

		unsigned char* data = stbi_load(Path.c_str(), &Width, &Height, &NRComponents, 0);

		if (data)
		{
			GLenum format = 0;
			if (NRComponents == 1)
			{
				format = GL_RED;
			}
			else if (NRComponents == 3)
			{
				format = GL_RGB;
			}
			else if (NRComponents == 4)
			{
				format = GL_RGBA;
			} 

			glBindTexture(GL_TEXTURE_2D, TexID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);

			for (const auto& [param, value] : ParamsMap)
			{
				glTexParameteri(GL_TEXTURE_2D, param, value);
			}

			Reg.emplace<TextureIDComponent>(ent, TID);
			Reg.emplace<TextureParamatersComponent>(ent, TPC);
			
		}
		else
		{
			HAMILTON_LOG(Graphics, Warning, "Unable to texture load file: %s", Path.c_str());
		}

		HAMILTON_LOG(Graphics, Info, "Loaded Texture: %s", Path.c_str());

		return ent;
	}



}