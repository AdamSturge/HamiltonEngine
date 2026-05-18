#pragma once
#include <PrecompiledHeader/Pch.h>

//#define STB_IMAGE_IMPLEMENTATION 
//#include "stb_image.h"

namespace HamiltonEngine::RenderingSystem
{
	struct TextureIDComponent {
		GLuint ID = 0;
	};

	struct TextureParamatersComponent {
		std::string Path;
		std::map<GLuint, GLuint> TextureParamatersMap;
	};

	const std::map<GLuint, GLuint> DEFAULT_TEXTURE_PARAMS{
		{GL_TEXTURE_WRAP_S, GL_REPEAT},
		{GL_TEXTURE_WRAP_T, GL_REPEAT},
		{GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
		{GL_TEXTURE_MAG_FILTER, GL_LINEAR}
	};

	entt::entity CreateTexture(std::string Path);
	entt::entity CreateTexture(std::string Path, std::map<GLuint, GLuint> ParamsMap);
}