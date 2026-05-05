#pragma once


namespace HamiltonEngine::OpenGL
{
	const int UNSET = -1;

	struct TransformComponent
	{
		Eigen::Vector3f Position;
		float RotationAngle;
		Eigen::Vector3f RotationAxis;
		Eigen::Vector3f Scale;
	};

	struct TextureComponent
	{
		GLuint ID;
		std::string FilePath;
		GLuint InternalFormat;
		GLuint Format;
		GLuint Type;
		bool FlipVertically;
		std::string Name;
	};

	struct ShaderComponent
	{
		GLuint ID;
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
		std::map<int, std::string> EnabledAttributes;
		// UniformMap: Name -> Location ?
		std::map<std::string, int> UniformMap;
	};

	struct OpenGLBuffersComponent
	{
		GLuint VBO = UNSET;
		GLuint VAO = UNSET;
		GLuint EBO = UNSET; // This is technically optional to use. its a good idea, but isn't strictly necessary
		// Could these be different components?
		GLenum DrawMode;
		GLint start;
		GLsizei count;
	};

	struct AssignedShader
	{
		entt::entity EntityID;
	};

	TextureComponent CreateTextureComponent(std::string Path, GLuint InternalFormat, GLuint Format, GLuint Type, bool FlipVertically);
	TransformComponent CreateTransformComponent();
	ShaderComponent CreateShaderComponent();
	OpenGLBuffersComponent CreateOpenGLBuffersComponent(bool CreateEBO);

	void CreateBasicTextures(); // This is mainly for testing purposes
	entt::entity CreateRenderableEntity(OpenGLBuffersComponent Buffs, TransformComponent Trans);

	void Render(GLint modelLoc);
	void RenderBuffer(OpenGLBuffersComponent Buffs, TransformComponent Trans, GLint ModelLoc);
	void RenderBuffer(OpenGLBuffersComponent Buffs, Eigen::Affine3f Trans, GLint ModelLoc);
	
}