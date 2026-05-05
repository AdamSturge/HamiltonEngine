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
//#include <OpenGL/Primatives/Cube.h>
#include "Configuration/Globals.h"
#include "Serialization/JsonSerialization.h"


// TODO: Make a sphere
// TODO: LIGHTING!

int main(int argc, char** argv)
{
	std::srand(std::time(0));
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	const char* StartingLevel = HamiltonEngine::Globals::StartingLevel.Get().c_str();
	HamiltonEngine::Serialization::DeserializeEnttRegistryFromJson(HamiltonEngine::Globals::Registry, StartingLevel);
	
	HamiltonEngine::ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
	HamiltonEngine::ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
	HamiltonEngine::ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");

	//HamiltonEngine::Physics::CreateParticleEntities();
	//HamiltonEngine::Physics::CreateRigidBodyEntities();

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

	HamiltonEngine::OpenGL::Shader simpleShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs",
		"Source\\Shaders\\fragmentShader.fs");
	simpleShader.use();
	simpleShader.setInt("texture1", 0); // This is the texture channel, FragShader.uniformName -> Texture Channel
	simpleShader.setInt("texture2", 1);

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

		//simpleShader.setFloat("TIME", CurTime);
		//simpleShader.setFloat("mixRatio", sin(CurTime));
		simpleShader.use();
		
		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");
		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		// Will Render anything with a TransformComponent and a OpenGLBuffersComponent
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		HamiltonEngine::OpenGL::Render(modelLoc);

		//glBindVertexArray(HamiltonEngine::Globals::PrimativesBuffers["cube"].VAO);
		auto RigidBodyView = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::RigidBodyStateComponent>();

		for (auto [Entity, StateC] : RigidBodyView.each())
		{

			HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"], StateC.Transform, modelLoc);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, StateC.Transform.data());

			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

		}

		// Use a different texture for the sping ends
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 2);

		auto Springs = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::SpringPotentialComponent>();
		Eigen::Vector3f SpringModelScale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
		int a = 0;
		for (auto [Entity, SpringComp] : Springs.each())
		{
			auto Parent = HamiltonEngine::Globals::Registry.get<HamiltonEngine::Physics::RigidBodyStateComponent>(SpringComp.ParentEntity);

			Eigen::Affine3f SpringWorldTransform = Parent.Transform.translate(SpringComp.AnchorPointBody)
																	.scale(SpringModelScale);
			
			HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"], SpringWorldTransform, modelLoc);
		}

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();

		printf("%d\n", glGetError());
	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();

	//constexpr bool OverwriteLevelFile = true;
	//HamiltonEngine::Serialization::SerializeEnttRegistryAsJson(HamiltonEngine::Globals::Registry, 
	//	"TestLevel.json", OverwriteLevelFile);
	return 0;
}