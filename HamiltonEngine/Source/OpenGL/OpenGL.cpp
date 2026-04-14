#include <PrecompiledHeader/Pch.h>
#include "OpenGL.h"
#include "Configuration/Globals.h"
#include "OpenGL/Texture.h"
#include <OpenGL/Utils.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

namespace HamiltonEngine::OpenGL
{

	TextureComponent CreateTextureComponent(std::string Path, GLuint InternalFormat, GLuint Format, GLuint Type, bool FlipVertically = false)
	{
		TextureComponent Tc;

		GLuint TexID;
		glGenTextures(1, &TexID); // OpenGL Texture ID
		glBindTexture(GL_TEXTURE_2D, TexID);

		Tc.ID = TexID;
		Tc.FilePath = Path;
		Tc.InternalFormat = InternalFormat;
		Tc.Format;
		Tc.Type;
		Tc.FlipVertically;

		int Width = 0, Height = 0, NRChannels;

		stbi_set_flip_vertically_on_load(FlipVertically);

		unsigned char* data = stbi_load(Path.c_str(), &Width, &Height, &NRChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, Type, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load textur at: " << Path << std::endl;
		}
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0); // unbind after creation; technically this is considered an antipattern

		return Tc;
	}
	
	TransformComponent CreateTransformComponent()
	{
		TransformComponent trans;
		trans.Position = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
		trans.RotationAngle = 0.0f;
		trans.RotationAxis = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
		trans.Scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

		return trans;
	}
	
	//ShaderComponent CreateShaderComponent()
	//{
	//	return;
	//}

	OpenGLBuffersComponent CreateOpenGLBuffersComponent(bool CreateEBO)
	{
		OpenGLBuffersComponent buffs;

		glGenVertexArrays(1, &buffs.VAO);
		glGenBuffers(1, &buffs.VBO);
		if (CreateEBO)
		{
			glGenBuffers(1, &buffs.EBO);
		}
		else
		{
			buffs.EBO = -1;
		}
		return buffs;

	}

	void CreateBasicTextures()
 	{

		entt::registry& Reg = HamiltonEngine::Globals::Registry;
		auto ent = Reg.create();

		std::string Tex1Path = "Assets\\Textures\\container.jpg";

		TextureComponent tex1 = CreateTextureComponent(Tex1Path, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false);
		glBindTexture(GL_TEXTURE_2D, tex1.ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		Reg.emplace<TextureComponent>(ent, tex1);
	
		ent = Reg.create();

		std::string Tex2Path  = "Assets\\Textures\\awesomeface.png";
		TextureComponent tex2 = CreateTextureComponent(Tex2Path,  GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false);
		glBindTexture(GL_TEXTURE_2D, tex2.ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Reg.emplace<TextureComponent>(ent, tex2);

	}

	entt::entity CreateRenderableEntity(OpenGLBuffersComponent Buffs, TransformComponent Trans)
	{
		entt::registry& reg = HamiltonEngine::Globals::Registry;
		const auto ent = reg.create();

		reg.emplace<OpenGLBuffersComponent>(ent, Buffs);
		reg.emplace<TransformComponent>(ent, Trans);

		return ent;
	}

	void Render(GLint modelLoc)
	{
		 auto RenderablesView = HamiltonEngine::Globals::Registry.view<const OpenGLBuffersComponent, const TransformComponent>();

		for (auto [Ent, Buffs, Trans] : RenderablesView.each())
		{
			//std::cout << entt::to_integral(Ent) << std::endl;
			RenderBuffer(Buffs, Trans, modelLoc);

		}

	}

	void RenderBuffer(OpenGLBuffersComponent Buffs, TransformComponent Trans, GLint ModelLoc)
	{
		
		Eigen::Affine3f Model = Eigen::Affine3f::Identity();

		glBindVertexArray(Buffs.VAO);

		// This should be an Eigen::Affine3f Matrix but this is less brain work to think about right now
		Model.translate(Trans.Position);
		Model.rotate(Eigen::AngleAxisf(
			DegToRad(Trans.RotationAngle),
			Trans.RotationAxis.normalized())
		);
		Model.scale(Trans.Scale);


		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, Model.data());

		if (Buffs.EBO == UNSET)
		{
			glDrawArrays(Buffs.DrawMode, Buffs.start, Buffs.count);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffs.EBO);
			glDrawElements(Buffs.DrawMode, Buffs.count, GL_UNSIGNED_INT, (void*) Buffs.start);
		}

	}

	void RenderBuffer(OpenGLBuffersComponent Buffs, Eigen::Affine3f Trans, GLint ModelLoc)
	{

		glBindVertexArray(Buffs.VAO);

		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, Trans.data());

		// TODO: Get this working with glGetIntegerv(GL_ARRAY_BUFFER_BINDING ...); 
		// Initial try didn't work.

		if (Buffs.EBO == UNSET)
		{
			glDrawArrays(Buffs.DrawMode, Buffs.start, Buffs.count);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffs.EBO);
			glDrawElements(Buffs.DrawMode, Buffs.count, GL_UNSIGNED_INT, (void*)Buffs.start);
		}

	}

}