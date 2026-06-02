#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"

#include "Logging/Logging.h"

#include "Physics/Systems/ParticleSystem.h"
#include "Physics/Systems/RigidBodySystem.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/SpringPotential.h"

#include <RenderingSystem/OpenGL.h>
#include <RenderingSystem/Window.h>
#include <RenderingSystem/Shader.h>
#include "RenderingSystem/Texture.h"

#include <iostream>

#include <RenderingSystem/SimpleShapes.h>
#include <RenderingSystem/Utils.h>
#include "RenderingSystem/Camera.h"

int main(int argc, char** argv)
{
	std::srand(std::time(0));
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");


	HamiltonEngine::RenderingSystem::SetupRenderingSystem();

	///

	HamiltonEngine::RenderingSystem::Shader lightingShader = HamiltonEngine::RenderingSystem::Shader::Shader("Source\\Shaders\\LightingShader\\vertexShader.vs",
		"Source\\Shaders\\LightingShader\\fragmentShader.fs");
	lightingShader.use();
	lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	HamiltonEngine::RenderingSystem::Shader lightShader = HamiltonEngine::RenderingSystem::Shader::Shader("source\\shaders\\LightShader\\vertexshader.vs",
			"source\\shaders\\LightShader\\fragmentshader.fs");

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
	HamiltonEngine::RenderingSystem::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;

	std::vector<float> WindowBackgroundColour = HamiltonEngine::ConfigurationVariable<std::vector<float>>("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
	float WindowBackgroundRed = WindowBackgroundColour[0];
	float WindowBackgroundGreen = WindowBackgroundColour[1];
	float WindowBackgroundBlue = WindowBackgroundColour[2];
		
	HamiltonEngine::Globals::FrameCount = 0;
	float CurTime = 0.0f;
	float OldTime = 0.0f;
	float DeltaTime = 0.0f;
	int CurWinHeight, CurWinLength;
	glfwGetWindowSize(HamiltonEngine::Globals::MainWindow, &CurWinHeight, &CurWinLength);
	float NearClip = HamiltonEngine::ConfigurationVariable("NearClipPlane", HamiltonEngine::RenderingSystem::DEFAULT_NEAR_CLIP);
	float FarClip = HamiltonEngine::ConfigurationVariable("FarClipPlane", HamiltonEngine::RenderingSystem::DEFAULT_FAR_CLIP);

	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f View;
	Eigen::Matrix4f Projection = HamiltonEngine::RenderingSystem::MakeFrustum(Camera.fov, (float)CurWinHeight / CurWinLength, NearClip, FarClip);

	if (HamiltonEngine::ConfigurationVariable<int>("CreateTestObjects", false))
	{
		CreateTestObjects();
	}

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

	Eigen::Vector3f LightColor = Eigen::Vector3f(0.5f, 0.5f, 0.5f);

	std::string TexturesPath = "Assets\\Textures\\";

	std::string DiffuseMapTexturePath = TexturesPath + "container2.png";

	entt::entity DiffuseMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(DiffuseMapTexturePath);

	std::string SpecularMapTexturePath = TexturesPath + "container2_specular.png";

	entt::entity SpecularMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(SpecularMapTexturePath);

	std::string EmissionMapTexturePath = TexturesPath + "matrix.jpg";

	entt::entity EmissionMapTextureEntity = HamiltonEngine::RenderingSystem::CreateTexture(EmissionMapTexturePath);


	while (!glfwWindowShouldClose(HamiltonEngine::Globals::MainWindow)) {
		++HamiltonEngine::Globals::FrameCount;

		// How long as it been since the last frame?
		OldTime = CurTime;
		CurTime = static_cast<float>(glfwGetTime());
		DeltaTime = CurTime - OldTime;

		// input
		HamiltonEngine::RenderingSystem::processInput(HamiltonEngine::Globals::MainWindow);
		HamiltonEngine::RenderingSystem::ProcessMovement(HamiltonEngine::Globals::MainWindow, DeltaTime);

		HamiltonEngine::Physics::ParticleSystem();
		HamiltonEngine::Physics::RigidBodySystem();

		// rendering
		HamiltonEngine::RenderingSystem::Tick();

		lightingShader.use();
		
		GLint modelLoc = glGetUniformLocation(lightingShader.ID, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.ID, "view");
		View = HamiltonEngine::RenderingSystem::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		GLint projLoc = glGetUniformLocation(lightingShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		LightColor = Eigen::Vector3f(sin(CurTime/2), sin(CurTime/3), sin(CurTime/4));
		LightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

		// Create a test cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(DiffuseMapTextureEntity).ID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(SpecularMapTextureEntity).ID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, HamiltonEngine::Globals::Registry.get<HamiltonEngine::RenderingSystem::TextureIDComponent>(EmissionMapTextureEntity).ID);

		lightingShader.setVec3("lightPos", LightObj.Position);
		lightingShader.setVec3("viewPos", Camera.CameraPosition);

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
		View = HamiltonEngine::RenderingSystem::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		modelLoc = glGetUniformLocation(lightShader.ID, "model");
		viewLoc = glGetUniformLocation(lightShader.ID, "view");
		projLoc = glGetUniformLocation(lightShader.ID, "projection");

		lightShader.setVec3("LightColor", LightColor);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		LightObj.Position = Eigen::Vector3f(LightOrbitRadius *cos(CurTime), LightOrbitRadius * cos(CurTime/2), LightOrbitRadius * sin(CurTime));;
		//LightObj.Position = Eigen::Vector3f(2.0f, 2.0f, 2.0f);

		HamiltonEngine::RenderingSystem::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["sphere"],
			LightObj, modelLoc);





		// swap buffers and call events
		glfwSwapBuffers(HamiltonEngine::Globals::MainWindow);
		glfwPollEvents();
	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();
	return 0;
}