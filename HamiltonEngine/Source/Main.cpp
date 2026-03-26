#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/Systems/ParticleSystem.h"
#include "Physics/Systems/RigidBodySystem.h"
#include "Physics/State/RigidBodyState.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <OpenGL/SimpleShapes.h>
#include <OpenGL/Utils.h>
#include "OpenGL/Camera.h"
#include <OpenGL/BasicShapes/Cube.h>
#include "OpenGL/BasicShapes/Shapes.h"
#include "OpenGL/BasicShapes/Shapes.h"

// TODO: Make a sphere
// TODO: LIGHTING!

int main(int argc, char** argv)
{
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

	// TODO: Move into using the ECS Systems
	GLuint cube_VAO, cube_VBO;
	
	HamiltonEngine::OpenGL::CreateUnitCube(&cube_VAO, &cube_VBO);
	HamiltonEngine::OpenGL::CreateBasicTextures();


	HamiltonEngine::OpenGL::Shader simpleShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs", 
																						 "Source\\Shaders\\fragmentShader.fs");

	simpleShader.use();
	simpleShader.setInt("texture1", 0); // This is the texture channel, FragShader.uniformName -> Texture Channel
	simpleShader.setInt("texture2", 1);

	// Setup and use the Camera
	HamiltonEngine::OpenGL::Camera NewCamera{
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION, // Some where in space
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT, // Camera is looking at this direction
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP, // Camera can change, but is +Z
			Eigen::Vector3f(0, 1.0f, 0.0f), // Right is +Y
			Eigen::Vector3f(0.0f, 0.0f, 1.0f), // Up is +Z
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH,
			HamiltonEngine::OpenGL::DEFAULT_FOV};

	HamiltonEngine::Globals::ActiveCamera = NewCamera;
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
	bool RenderDefaultCubes = false; // HamiltonEngine::ConfigurationVariable<bool>("RenderDefaultCubes", false);

	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f View;
	Eigen::Matrix4f Projection = HamiltonEngine::OpenGL::MakeFrustum(Camera.fov, (float)WindowHeight / WindowWidth, NearClip, FarClip);

	
	//HamiltonEngine::OpenGL::TestTetras(5);
	//HamiltonEngine::OpenGL::TestCubes(5);

	GLuint tetra_VAO, tetra_VBO;
	
	HamiltonEngine::OpenGL::CreateTetra(&tetra_VAO, &tetra_VBO);

	HamiltonEngine::OpenGL::OpenGLBuffersComponent testBuffs;
	HamiltonEngine::OpenGL::TransformComponent testTrans;
	entt::entity testEnt = HamiltonEngine::OpenGL::CreateCube(RandomTransformComponent());

	testBuffs = HamiltonEngine::Globals::Registry.get<HamiltonEngine::OpenGL::OpenGLBuffersComponent>(testEnt);
	testTrans = HamiltonEngine::Globals::Registry.get<HamiltonEngine::OpenGL::TransformComponent>(testEnt);

	testTrans.Position = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

	PrintOpenGLBufferComponentData(testBuffs);
	PrintTransformComponent(testTrans);


	glGetError(); // clear errors before we start

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
		std::cout << glGetError() << std::endl;

//		simpleShader.setFloat("TIME", CurTime);
//		simpleShader.setFloat("mixRatio", sin(CurTime));
		simpleShader.use();

		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");
		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());

		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		glBindVertexArray(cube_VAO);
		auto RigidBodyView = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::RigidBodyStateComponent>();
		std::cout << glGetError() << std::endl;

		for (auto [Entity, StateC] : RigidBodyView.each())
		{
			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, StateC.Transform.data());

			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

		}

		//HamiltonEngine::OpenGL::Render(modelLoc);

		//glBindVertexArray(testBuffs.VAO);

		// This bit should move into the shader(s)
		//Model.translate(testTrans.Position);
		//Model.rotate(Eigen::AngleAxisf(
		//	DegToRad(testTrans.RotationAngle),
		//	testTrans.RotationAxis.normalized())
		//);
		//Model.scale(testTrans.Scale);
		glBindVertexArray(testBuffs.VAO);


		//glBindVertexArray(tetra_VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testBuffs.EBO);
		//glDrawElements(testBuffs.DrawMode, testBuffs.count, GL_UNSIGNED_INT, (void*)testBuffs.start);
		//glDrawElements(testBuffs.DrawMode, testBuffs.count, GL_UNSIGNED_INT, (void*) testBuffs.start);


		if (testBuffs.EBO == -1)
		{
			glDrawArrays(testBuffs.DrawMode, testBuffs.start, testBuffs.count);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testBuffs.EBO);
			glDrawElements(testBuffs.DrawMode, testBuffs.count, GL_UNSIGNED_INT, (void*)testBuffs.start);
		}


		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();


	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();
	return 0;
}