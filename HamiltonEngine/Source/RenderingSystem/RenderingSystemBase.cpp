#include <PrecompiledHeader/Pch.h>
#include "Configuration/Globals.h"
#include "RenderingSystemBase.h"
#include "Window.h"
#include <RenderingSystem/Utils.h>
#include "RenderingSystem/Texture.h"
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace {
	float WindowBackgroundRed;
	float WindowBackgroundGreen;
	float WindowBackgroundBlue;

	float NearClip;
	float FarClip;

	Eigen::Affine3f Model;
	Eigen::Matrix4f View;
	Eigen::Matrix4f Projection;

	HamiltonEngine::RenderingSystem::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;
	HamiltonEngine::RenderingSystem::Shader lightingShader;
	HamiltonEngine::RenderingSystem::Shader lightShader;

	HamiltonEngine::RenderingSystem::TransformComponent TestObj {
		Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(1.0f, 1.0f, 1.0f)
	};

	HamiltonEngine::RenderingSystem::TransformComponent TestObj2 {
		Eigen::Vector3f(0.0f, -3.0f, 0.0f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(1.0f, 1.0f, 1.0f)
	};

	HamiltonEngine::RenderingSystem::TransformComponent LightObj {
		Eigen::Vector3f(3.0f, 0.0f, 1.5f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(0.5f, 0.5f, 0.5f)
	};

	int LightOrbitRadius = 3.0f;

	//HamiltonEngine::RenderingSystem::Camera& cam = HamiltonEngine::RenderingSystem::Camera::Camera();

	Eigen::Vector3f LightColor = Eigen::Vector3f(0.5f, 0.5f, 0.5f);

	entt::entity DiffuseMapTextureEntity;
	entt::entity SpecularMapTextureEntity;
	entt::entity EmissionMapTextureEntity;
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

		// This is some data loading that will need to be handled later by a RenderingSystemManager?

		if (HamiltonEngine::ConfigurationVariable<int>("CreateTestObjects", false))
		{
			CreateTestObjects();
		}

		PopulatePrimativeMap();
		std::vector<float> WindowBackgroundColour = HamiltonEngine::ConfigurationVariable<std::vector<float>>("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
		WindowBackgroundRed = WindowBackgroundColour[0];
		WindowBackgroundGreen = WindowBackgroundColour[1];
		WindowBackgroundBlue = WindowBackgroundColour[2];

		NearClip = HamiltonEngine::ConfigurationVariable("NearClipPlane", HamiltonEngine::RenderingSystem::DEFAULT_NEAR_CLIP);
		FarClip = HamiltonEngine::ConfigurationVariable("FarClipPlane", HamiltonEngine::RenderingSystem::DEFAULT_FAR_CLIP);

		SetupDefaultCamera();
		LearnOpenGLTutorialObjectSetup();

		
		Model = Eigen::Affine3f::Identity();
		//View;

		int CurWinHeight, CurWinLength;
		glfwGetWindowSize(HamiltonEngine::Globals::MainWindow, &CurWinHeight, &CurWinLength);

		Projection = HamiltonEngine::RenderingSystem::MakeFrustum(HamiltonEngine::Globals::ActiveCamera.fov, (float)CurWinHeight / CurWinLength, NearClip, FarClip);

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

		//using namespace entt::literals;
		//HamiltonEngine::Globals::Registry.ctx().emplace_as<HamiltonEngine::RenderingSystem::Camera>("ActiveCamera"_hs, HamiltonEngine::Globals::ActiveCamera);

	}

	void LearnOpenGLTutorialObjectSetup() {

		lightingShader = HamiltonEngine::RenderingSystem::Shader::Shader("Source\\Shaders\\LightingShader\\vertexShader.vs",
			"Source\\Shaders\\LightingShader\\fragmentShader.fs");
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		lightShader = HamiltonEngine::RenderingSystem::Shader::Shader("source\\shaders\\LightShader\\vertexshader.vs",
			"source\\shaders\\LightShader\\fragmentshader.fs");

		

		std::string TexturesPath = "Assets\\Textures\\";

		std::string DiffuseMapTexturePath = TexturesPath + "container2.png";

		DiffuseMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(DiffuseMapTexturePath);

		std::string SpecularMapTexturePath = TexturesPath + "container2_specular.png";

		SpecularMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(SpecularMapTexturePath);

		std::string EmissionMapTexturePath = TexturesPath + "matrix.jpg";

		EmissionMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(EmissionMapTexturePath);
	}

	void Tick()
	{
		// We do this every frame,
		HamiltonEngine::RenderingSystem::Camera& cam = HamiltonEngine::Globals::ActiveCamera;

		View = HamiltonEngine::RenderingSystem::LookAt(cam.CameraPosition, cam.CameraPosition + cam.CameraFront, cam.WorldUp);

		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightingShader.use();

		GLint modelLoc = glGetUniformLocation(lightingShader.ID, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.ID, "view");
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		GLint projLoc = glGetUniformLocation(lightingShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		//LightColor = Eigen::Vector3f(sin(CurTime / 2), sin(CurTime / 3), sin(CurTime / 4));
		LightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(DiffuseMapTextureEntity).ID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(SpecularMapTextureEntity).ID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(EmissionMapTextureEntity).ID);

		lightingShader.setVec3("lightPos", LightObj.Position);
		lightingShader.setVec3("viewPos", cam.CameraPosition);

		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 256.0f);
		lightingShader.setInt("material.diffuseMap", 0);
		lightingShader.setInt("material.specularMap", 1);
		lightingShader.setInt("material.emissionMap", 2);

		//lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		//lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//lightingShader.setFloat("material.shininess", 32.0f);

		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", LightColor);
		lightingShader.setVec3("light.specular", LightColor);




		HamiltonEngine::RenderingSystem::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"],
			TestObj, modelLoc);

		HamiltonEngine::RenderingSystem::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["sphere"],
			TestObj2, modelLoc);


		// light
		lightShader.use();
		View = HamiltonEngine::RenderingSystem::LookAt(cam.CameraPosition, cam.CameraPosition + cam.CameraFront, cam.WorldUp);
		modelLoc = glGetUniformLocation(lightShader.ID, "model");
		viewLoc = glGetUniformLocation(lightShader.ID, "view");
		projLoc = glGetUniformLocation(lightShader.ID, "projection");

		lightShader.setVec3("LightColor", LightColor);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		//LightObj.Position = Eigen::Vector3f(LightOrbitRadius * cos(CurTime), LightOrbitRadius * cos(CurTime / 2), LightOrbitRadius * sin(CurTime));;
		//LightObj.Position = Eigen::Vector3f(2.0f, 2.0f, 2.0f);

		HamiltonEngine::RenderingSystem::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["sphere"],
			LightObj, modelLoc);


		Render(modelLoc);


		// swap buffers and call events
		glfwSwapBuffers(HamiltonEngine::Globals::MainWindow);
		glfwPollEvents();
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