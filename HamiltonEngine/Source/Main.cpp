#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/Systems/ParticleSystem.h"

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

void ProcessMovement(GLFWwindow* window, HamiltonEngine::OpenGL::Camera* camera, float DeltaTime);
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	GLuint WindowWidth = HamiltonEngine::Globals::WindowWidth;
	GLuint WindowHeight = HamiltonEngine::Globals::WindowHeight;
	std::string WindowName = HamiltonEngine::Globals::WindowName;

	entt::registry Registry;
	HamiltonEngine::Physics::CreateParticlesEntities(Registry);

	glfwInit(); // Initialize OpenGL
	GLFWwindow* window = HamiltonEngine::OpenGL::createWindow(WindowHeight, WindowWidth, WindowName.c_str());
	glfwSetCursorPos(window, HamiltonEngine::Globals::WindowHeight / 2, HamiltonEngine::Globals::WindowWidth / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (window == NULL)
	{
		return -1;
	}

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
	

	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f Projection = HamiltonEngine::OpenGL::MakeFrustum(HamiltonEngine::Globals::camera.fov, (float) WindowHeight / WindowWidth, 0.1f, 100.0f);
	Eigen::Matrix4f View = HamiltonEngine::OpenGL::LookAt(HamiltonEngine::Globals::camera.CameraPosition, Eigen::Vector3f::Zero(), Eigen::Vector3f::UnitY());

	float WindowBackgroundRed = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[0];
	float WindowBackgroundGreen = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[1];
	float WindowBackgroundBlue = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[2];
	
	float yaw = DegToRad( -90.f);
	Eigen::Vector3f direction = Eigen::Vector3f::Zero();
	
	glEnable(GL_DEPTH_TEST);

	HamiltonEngine::Globals::FrameCount = 0;
	float curTime = 0.0f;
	float oldTime = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		++HamiltonEngine::Globals::FrameCount;

		// How long as it been since the last frame?
		oldTime = curTime;
		curTime = static_cast<float>(glfwGetTime());
		deltaTime = curTime - oldTime;

		// input
		HamiltonEngine::OpenGL::processInput(window);
		ProcessMovement(window, &HamiltonEngine::Globals::camera, deltaTime);
		glfwSetCursorPosCallback(window, HamiltonEngine::OpenGL::mouse_callback);
		glfwSetScrollCallback(window, HamiltonEngine::OpenGL::scroll_callback);

		HamiltonEngine::Physics::ParticleSystem(Registry);

		// rendering
		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1c.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2c.ID);


		simpleShader.setFloat("TIME", curTime);
		simpleShader.setFloat("mixRatio", sin(curTime));
		simpleShader.use();

		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");

		const float radius = 20.0f;
		const float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		const float camZ = static_cast<float>(cos(glfwGetTime()) * radius);

		Projection = HamiltonEngine::OpenGL::MakeFrustum(HamiltonEngine::Globals::camera.fov, (float)WindowHeight / WindowWidth, 0.1f, 100.0f);
		View = HamiltonEngine::OpenGL::LookAt(HamiltonEngine::Globals::camera.CameraPosition,
					  HamiltonEngine::Globals::camera.CameraPosition + HamiltonEngine::Globals::camera.CameraFront, 
					  HamiltonEngine::Globals::camera.CameraUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());

		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");


		glUniformMatrix4fv(projLoc, 1, GL_FALSE,
			Projection.data()
			);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());


		glBindVertexArray(cube_VAO);
		for (int i = 0; i < 1; i++)
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


void ProcessMovement(GLFWwindow* window, HamiltonEngine::OpenGL::Camera* camera, float DeltaTime)
{
	const float CameraSpeed = 2.5f * DeltaTime;

	// Release the Camera
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		{
			glfwSetCursorPos(window, HamiltonEngine::Globals::WindowHeight / 2, HamiltonEngine::Globals::WindowWidth / 2);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

	// Reset to the default Camera Position
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		HamiltonEngine::Globals::camera.CameraFront = HamiltonEngine::Globals::DefaultCameraFront;
		HamiltonEngine::Globals::camera.CameraPosition = HamiltonEngine::Globals::DefaultCameraPosition;
		HamiltonEngine::Globals::camera.CameraUp = HamiltonEngine::Globals::DefaultCameraUp;
	}


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->CameraPosition += CameraSpeed * camera->CameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->CameraPosition -= CameraSpeed * camera->CameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->CameraPosition -= camera->CameraFront.cross(camera->CameraUp).normalized() * CameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->CameraPosition += camera->CameraFront.cross(camera->CameraUp).normalized() * CameraSpeed;
	}
}


