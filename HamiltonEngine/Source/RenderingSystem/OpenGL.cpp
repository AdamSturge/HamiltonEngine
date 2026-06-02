#include <PrecompiledHeader/Pch.h>
#include "Configuration/Globals.h"
#include "OpenGL.h"
#include "Window.h"
#include <RenderingSystem/Utils.h>
#include "RenderingSystem/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace {
	float WindowBackgroundRed;
	float WindowBackgroundGreen;
	float WindowBackgroundBlue;
}

namespace HamiltonEngine::RenderingSystem
{
	bool SetupRenderingSystem()
	{
		HamiltonEngine::ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
		HamiltonEngine::ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
		HamiltonEngine::ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");


		if (!glfwInit()) // Initialize OpenGL
		{
			HAMILTON_LOG(Graphics, Critical, "OpenGL failed to initialize. Exiting.")
			return false;
		}

		GLFWwindow* window = HamiltonEngine::RenderingSystem::createWindow(WindowHeight, WindowWidth, ((std::string)WindowName).c_str());
		HamiltonEngine::Globals::MainWindow = window;

		glfwSetCursorPos(window, WindowHeight / 2, WindowWidth / 2);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glEnable(GL_DEPTH_TEST);

		if (window == NULL)
		{
			return false;
		}

		PopulatePrimativeMap();
		std::vector<float> WindowBackgroundColour = HamiltonEngine::ConfigurationVariable<std::vector<float>>("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
		WindowBackgroundRed = WindowBackgroundColour[0];
		WindowBackgroundGreen = WindowBackgroundColour[1];
		WindowBackgroundBlue = WindowBackgroundColour[2];

		SetupDefaultCamera();

		return true;

	}

	void SetupDefaultCamera()
	{
		// Setup and use the Camera
		// This conversion is kind of gross due to the Camera Vectors being stored as Eigen::Vector3f and no direct conversion
		std::vector<float> CameraStartPositionVec = HamiltonEngine::ConfigurationVariable<std::vector<float>>("CameraStartPosition", { HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_POSITION.x(),
																																	   HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_POSITION.y(),
																																	   HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_POSITION.z() });
		Eigen::Vector3f CameraStartPosition = Eigen::Vector3f(CameraStartPositionVec.data());

		// Setup and use the Camera
		HamiltonEngine::Globals::ActiveCamera = HamiltonEngine::RenderingSystem::Camera{
				CameraStartPosition, // Some where in space
				HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_FRONT, // Camera is looking at this direction
				HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_UP, // Camera can change, but is +Z
				Eigen::Vector3f(0, 1.0f, 0.0f), // Right is +Y
				Eigen::Vector3f(0.0f, 0.0f, 1.0f), // Up is +Z
				HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_YAW,
				HamiltonEngine::RenderingSystem::DEFAULT_CAMERA_PITCH,
				HamiltonEngine::RenderingSystem::DEFAULT_FOV };

	}

	void LearnOpenGLTutorialObjectSetup() {

	}

	void Tick()
	{
		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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