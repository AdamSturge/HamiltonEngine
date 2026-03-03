#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/Systems/ParticleSystem.h"
#include "Physics/Systems/RigidBodySystem.h"
#include "Physics/State/RigidBodyState.h"

#include <OpenGl/OpenGL.h>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include "OpenGL/Texture.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <OpenGL/SimpleShapes.h>
#include <OpenGL/Utils.h>
#include "OpenGL/Camera.h"
#include "Configuration/Globals.h"

// TODO: Create an easy way to instantiate boxes 
// TODO: Make a sphere
// TODO: LIGHTING!

namespace
{
	bool RenderDefaultCubes = false;

}

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

	if (window == NULL)
	{
		return -1;
	}

	// TODO: Move into using the ECS Systems
	GLuint cube_VAO, cube_VBO;
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	glCreateBuffers(1, &cube_VBO);	

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICIES), CUBE_VERTICIES, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	HamiltonEngine::OpenGL::Texture texture1c = HamiltonEngine::OpenGL::Texture::Texture("container.jpg", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	texture1c.setActive();
	texture1c.setTextureOption(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	texture1c.setTextureOption(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	texture1c.setTextureOption(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture1c.setTextureOption(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	HamiltonEngine::OpenGL::Texture texture2c = HamiltonEngine::OpenGL::Texture::Texture("awesomeface.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	texture2c.setActive();
	texture2c.setTextureOption(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture2c.setTextureOption(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture2c.setTextureOption(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture2c.setTextureOption(GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	HamiltonEngine::OpenGL::Shader simpleShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs", 
																						 "Source\\Shaders\\fragmentShader.fs");

	simpleShader.use();
	simpleShader.setInt("texture1", 0);
	simpleShader.setInt("texture2", 1);
	
	// Setup and use the Camera
	HamiltonEngine::OpenGL::Camera NewCamera{
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW,
			HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH,
			// Pitch if we need it
			HamiltonEngine::OpenGL::DEFAULT_FOV};
	HamiltonEngine::Globals::ActiveCamera = NewCamera;
	HamiltonEngine::OpenGL::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;

	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f Projection = HamiltonEngine::OpenGL::MakeFrustum(Camera.fov, (float) WindowHeight / WindowWidth, 0.1f, 100.0f);
	Eigen::Matrix4f View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Eigen::Vector3f::Zero(), Eigen::Vector3f::UnitY());

	std::vector<float> WindowBackgroundColour = HamiltonEngine::ConfigurationVariable<std::vector<float>>("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
	float WindowBackgroundRed = WindowBackgroundColour[0];
	float WindowBackgroundGreen = WindowBackgroundColour[1];
	float WindowBackgroundBlue = WindowBackgroundColour[2];
		
	glEnable(GL_DEPTH_TEST);

	HamiltonEngine::Globals::FrameCount = 0;
	float CurTime = 0.0f;
	float OldTime = 0.0f;
	float DeltaTime = 0.0f;
	float NearClip = HamiltonEngine::ConfigurationVariable("NearClipPlane", HamiltonEngine::OpenGL::DEFAULT_NEAR_CLIP);
	float FarClip = HamiltonEngine::ConfigurationVariable("FarClipPlane", HamiltonEngine::OpenGL::DEFAULT_FAR_CLIP);

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
		glBindTexture(GL_TEXTURE_2D, texture1c.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2c.ID);


		simpleShader.setFloat("TIME", CurTime);
		simpleShader.setFloat("mixRatio", sin(CurTime));
		simpleShader.use();

		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");


		Projection = HamiltonEngine::OpenGL::MakeFrustum(Camera.fov,
														(float)WindowHeight / WindowWidth, 
														NearClip,
														FarClip);


		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition,
											  Camera.CameraPosition + Camera.CameraFront,
												Camera.CameraUp);


		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());

		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");


		glUniformMatrix4fv(projLoc, 1, GL_FALSE,
			Projection.data()
			);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());


		glBindVertexArray(cube_VAO);
		auto RigidBodyView = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::RigidBodyStateComponent>();


		// A scattering of default cubes
		RenderDefaultCubes = HamiltonEngine::ConfigurationVariable<bool>("RenderDefaultCubes", false);
		if (RenderDefaultCubes)
		{
			for (int i = 0; i < 10; i++)
			{
				Model = Eigen::Affine3f::Identity();
				Model.translate(cubePositions[i]);

				float angle = 20.0f * i;
				Model.rotate(Eigen::AngleAxisf(
					DegToRad(angle)
					, Eigen::Vector3f(0.5f, 1.0f, 0.0f).normalized()));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

				glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

			}
		}



		for (auto [Entity, StateC] : RigidBodyView.each())
		{
			

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, StateC.Transform.data());

			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	glfwTerminate();
	return 0;
}