#pragma once
#include "OpenGL.h"
#include "Texture.h"
#include <iostream>


namespace HamiltonEngine::OpenGL
{
	class Texture
	{
	public:
		unsigned int ID;

		Texture(const char* path, unsigned int internalformat, unsigned int format, unsigned int type);
		void setTextureOption(unsigned int texOption, unsigned int setting);
		void setActive();

	};
}