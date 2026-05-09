#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"

#include "Logging/Logging.h"

#include "Physics/Systems/ParticleSystem.h"
#include "Physics/Systems/RigidBodySystem.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/SpringPotential.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include "OpenGL/Texture.h"

#include <iostream>

#include <OpenGL/SimpleShapes.h>
#include <OpenGL/Utils.h>
#include "OpenGL/Camera.h"

int main(int argc, char** argv)
{
	std::srand(std::time(0));
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	HamiltonEngine::ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
	HamiltonEngine::ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
	HamiltonEngine::ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");

	HamiltonEngine::Physics::CreateParticleEntities();
	HamiltonEngine::Physics::CreateRigidBodyEntities();

	glfwInit(); // Initialize OpenGL
	
	GLFWwindow* window = HamiltonEngine::OpenGL::createWindow(WindowHeight, WindowWidth, ((std::string)WindowName).c_str());

	glfwSetCursorPos(window, WindowHeight / 2, WindowWidth / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	if (window == NULL)
	{
		return -1;
	}

	HamiltonEngine::OpenGL::CreateBasicTextures();
	PopulatePrimativeMap();

	HamiltonEngine::OpenGL::Shader lightingShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\LightingShader\\vertexShader.vs",
		"Source\\Shaders\\LightingShader\\fragmentShader.fs");
	lightingShader.use();
	lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	HamiltonEngine::OpenGL::Shader lightShader = HamiltonEngine::OpenGL::Shader::Shader("source\\shaders\\LightShader\\vertexshader.vs",
			"source\\shaders\\LightShader\\fragmentshader.fs");

	// Setup and use the Camera
	// This conversion is kind of gross due to the Camera Vectors being stored as Eigen::Vector3f and no direct conversion
	std::vector<float> CameraStartPositionVec = HamiltonEngine::ConfigurationVariable<std::vector<float>>("CameraStartPosition", { HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION.x(),
																																   HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION.y(),
																																   HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION.z() });
	Eigen::Vector3f CameraStartPosition = Eigen::Vector3f(CameraStartPositionVec.data());

	// Setup and use the Camera
	HamiltonEngine::Globals::ActiveCamera = HamiltonEngine::OpenGL::Camera{
			CameraStartPosition, // Some where in space
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT, // Camera is looking at this direction
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP, // Camera can change, but is +Z
			Eigen::Vector3f(0, 1.0f, 0.0f), // Right is +Y
			Eigen::Vector3f(0.0f, 0.0f, 1.0f), // Up is +Z
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH,
			HamiltonEngine::OpenGL::DEFAULT_FOV };
	HamiltonEngine::OpenGL::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;

	std::vector<float> WindowBackgroundColour = HamiltonEngine::ConfigurationVariable<std::vector<float>>("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
	float WindowBackgroundRed = WindowBackgroundColour[0];
	float WindowBackgroundGreen = WindowBackgroundColour[1];
	float WindowBackgroundBlue = WindowBackgroundColour[2];
		
	HamiltonEngine::Globals::FrameCount = 0;
	float CurTime = 0.0f;
	float OldTime = 0.0f;
	float DeltaTime = 0.0f;
	float NearClip = HamiltonEngine::ConfigurationVariable("NearClipPlane", HamiltonEngine::OpenGL::DEFAULT_NEAR_CLIP);
	float FarClip = HamiltonEngine::ConfigurationVariable("FarClipPlane", HamiltonEngine::OpenGL::DEFAULT_FAR_CLIP);

	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f View;
	Eigen::Matrix4f Projection = HamiltonEngine::OpenGL::MakeFrustum(Camera.fov, (float)WindowHeight / WindowWidth, NearClip, FarClip);

	if (HamiltonEngine::ConfigurationVariable<int>("CreateTestObjects", false))
	{
		CreateTestObjects();
	}

	HamiltonEngine::OpenGL::TransformComponent TestObj {
		Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(1.0f, 1.0f, 1.0f)
	};

	HamiltonEngine::OpenGL::TransformComponent TestObj2 {
		Eigen::Vector3f(0.0f, -3.0f, 0.0f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(1.0f, 1.0f, 1.0f)
	};

	HamiltonEngine::OpenGL::TransformComponent LightObj {
		Eigen::Vector3f(3.0f, 0.0f, 1.5f),
			0.0f,
			Eigen::Vector3f(0.0f, 0.0f, 0.0f),
			Eigen::Vector3f(0.5f, 0.5f, 0.5f)
	};

	int LightOrbitRadius = 3.0f;

	Eigen::Vector3f LightColor = Eigen::Vector3f(0.5f, 0.5f, 0.5f);

	while (!glfwWindowShouldClose(window)) {
		++HamiltonEngine::Globals::FrameCount;

		// How long as it been since the last frame?
		OldTime = CurTime;
		CurTime = static_cast<float>(glfwGetTime());
		DeltaTime = CurTime - OldTime;

		// input
		HamiltonEngine::OpenGL::processInput(window);
		HamiltonEngine::OpenGL::ProcessMovement(window, DeltaTime);

		HamiltonEngine::Physics::ParticleSystem();
		HamiltonEngine::Physics::RigidBodySystem();

		// rendering
		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 2);

		lightingShader.use();
		
		GLint modelLoc = glGetUniformLocation(lightingShader.ID, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.ID, "view");
		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		GLint projLoc = glGetUniformLocation(lightingShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		// Will Render anything with a TransformComponent and a OpenGLBuffersComponent
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		//HamiltonEngine::OpenGL::Render(modelLoc);

		LightColor = Eigen::Vector3f(sin(CurTime/2), sin(CurTime/3), sin(CurTime/4));

		// Create a test cube
		lightingShader.setVec3("lightPos", LightObj.Position);
		lightingShader.setVec3("viewPos", Camera.CameraPosition);

		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);

		//lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		//lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//lightingShader.setFloat("material.shininess", 32.0f);
		
		lightingShader.setVec3("light.ambient", LightColor);
		lightingShader.setVec3("light.diffuse", LightColor);
		lightingShader.setVec3("light.specular", LightColor);

		HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"],
			TestObj, modelLoc);

		HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["sphere"],
			TestObj2, modelLoc);


		// light
		lightShader.use();
		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		modelLoc = glGetUniformLocation(lightShader.ID, "model");
		viewLoc = glGetUniformLocation(lightShader.ID, "view");
		projLoc = glGetUniformLocation(lightShader.ID, "projection");

		lightShader.setVec3("LightColor", LightColor);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		LightObj.Position = Eigen::Vector3f(LightOrbitRadius *sin(CurTime), LightOrbitRadius * cos(CurTime), LightObj.Position.z());;

		HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["sphere"],
			LightObj, modelLoc);





		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();
	return 0;
}