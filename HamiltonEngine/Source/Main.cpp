#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Physics/PhysicsState.h"
#include "Configuration/ConfigurationVariable.h"
#include "Physics/SymplecticEulerSystem.h"

#include "entt/entt.hpp"
#include <OpenGl/OpenGL.h>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include "OpenGL/Texture.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Eigen/Dense>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/SimpleShapes.h>
#include <OpenGL/Utils.h>
#include "OpenGL/Camera.h"
#include "Configuration/Globals.h"

float deltaTime = 0.0f;

namespace HamiltonEngine
{
	ConfigurationVariable<int> NumEntities("NumEntities", 10);
	
	void CreatePhysicsEntities(entt::registry& Registry)
	{
		for (int EntityIndex = 0; EntityIndex < NumEntities; ++EntityIndex) 
		{
			entt::entity Entity = Registry.create();
			Registry.emplace<Physics::PositionComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::LinearMomentumComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::MassComponent>(Entity, 1.0f);
			Registry.emplace<Physics::OrientationComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::AngularMomentumComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::InertiaTensorComponent>(Entity, Eigen::Matrix3f::Identity());
		}
	}
}


void ProcessMovement(GLFWwindow* window, HamiltonEngine::OpenGL::Camera* camera)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float CameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->CameraPosition += CameraSpeed * camera->CameraFront;
		std::cout << "W" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->CameraPosition -= CameraSpeed * camera->CameraFront;
		std::cout << "S" << std::endl;
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->CameraPosition -= camera->CameraFront.cross(camera->CameraUp).normalized() * CameraSpeed;
		std::cout << "A" << std::endl;
	}
		
		// CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->CameraPosition += camera->CameraFront.cross(camera->CameraUp).normalized() * CameraSpeed;
		std::cout << "D" << std::endl;
	}
		
		//CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;


}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	GLuint WindowWidth = HamiltonEngine::Globals::WindowWidth;
	GLuint WindowHeight = HamiltonEngine::Globals::WindowHeight;

	entt::registry Registry;
	HamiltonEngine::CreatePhysicsEntities(Registry);

	glfwInit(); // Initialize OpenGL
	GLFWwindow* window = HamiltonEngine::OpenGL::createWindow(WindowHeight, WindowWidth, "MyWindow");

	if (window == NULL)
	{
		return -1;
	}

	float red = 0.2f;
	float green = 0.3f;
	float blue = 0.3f;
	

	unsigned int VAO, VBO, EBO;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	GLuint cube_VAO, cube_VBO;
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	glCreateBuffers(1, &cube_VBO);	

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICIES), CUBE_VERTICIES, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

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

	int fb_width = 800;
	int fb_height = 600;

	//const float radius = 10.0f;
	//float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	//float camZ = static_cast<float>(cos(glfwGetTime()) * radius);

	//Eigen::Matrix4f e_view2 = LookAt(Eigen::Vector3f(camX, 0.0f, camZ), 
	//								Eigen::Vector3f::Zero(),
	//								Eigen::Vector3f::UnitY());


	
	HamiltonEngine::OpenGL::Camera camera{
			Eigen::Vector3f(0.0f, 0.0f, 3.0f),
			Eigen::Vector3f(0.0f, 0.0f, -1.0f),
			Eigen::Vector3f(0.0f, 1.0f, 0.0f)
	};

	float fov = 45.0f;



	Eigen::Affine3f Model = Eigen::Affine3f::Identity();
	Eigen::Matrix4f Projection = MakeFrustum(fov, WindowHeight / WindowWidth, 0.1f, 100.0f);
	Eigen::Matrix4f View = LookAt(camera.CameraPosition, Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f::UnitY());

	float WindowBackgroundRed = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[0];
	float WindowBackgroundGreen = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[1];
	float WindowBackgroundBlue = ((std::vector<float>)HamiltonEngine::Globals::BackgroundColorRGB)[2];
	
	
	glEnable(GL_DEPTH_TEST);
	float curTime = 0.0f;
	float oldTime = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		// input
		HamiltonEngine::OpenGL::processInput(window);
		ProcessMovement(window, &camera);

		// Run Physics Sim
		auto PhysicsSimView = Registry.view<
			HamiltonEngine::Physics::PositionComponent,
			HamiltonEngine::Physics::LinearMomentumComponent>();

		for (auto [Entity, PosC, LinMomC] : PhysicsSimView.each())
		{
			HamiltonEngine::Physics::SymplecticEulerSystem(PosC, LinMomC);
		}

		// rendering
		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1c.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2c.ID);

		oldTime = curTime;
		curTime = static_cast<float>(glfwGetTime());
		deltaTime = curTime - oldTime;
		simpleShader.setFloat("TIME", curTime);
		simpleShader.setFloat("mixRatio", sin(curTime));
		simpleShader.use();

		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");

		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;

		//Eigen::Matrix4f e_view2 = LookAt(Eigen::Vector3f(camX, 0.0f, camZ),
		//	Eigen::Vector3f::Zero(),
		//	Eigen::Vector3f::UnitY());

		//e_view2 = LookAt(CameraPos, CameraPos + CameraFront, CameraUp);

		View = LookAt(camera.CameraPosition, camera.CameraPosition + camera.CameraFront, camera.CameraUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());

		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");

		//projection = glm::perspective(glm::radians(fov * sin(curTime)), 800.0f / 600.0f, 0.1f, 100.0f);

		glUniformMatrix4fv(projLoc, 1, GL_FALSE,
			//glm::value_ptr(projection)
			Projection.data()
			);


		//e_model.rotate(Eigen::AngleAxisf(
		//	//DegToRad(5.0f * curTime)
		//	DegToRad(50) * deltaTime
		//	, Eigen::Vector3f(0.5f, 1.0f, 0.0f).normalized()));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());									// Need to normalize the rotation vector!


		glBindVertexArray(cube_VAO);
		for (int i = 0; i < 10; i++)
		{
			Model = Eigen::Affine3f::Identity();
			Model.translate(cubePositions[i]);

			float angle = 20.0f * i;
				Model.rotate(Eigen::AngleAxisf(
					DegToRad(angle) 
					, Eigen::Vector3f(0.5f, 1.0f, 0.0f).normalized()));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		//// This is using Eigen
		//amat = Eigen::Affine3f::Identity();
		//amat.translate(Eigen::Vector3f(-0.5, 0.5f, 0.0f));
		//amat.scale(Eigen::Vector3f(abs(sin(curTime)), abs(sin(curTime)), 0.0f));

		///* This is using glm
		//trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		//trans = glm::scale(trans, glm::vec3(abs(sin(curTime)), abs(sin(curTime)), 0.0f));
		//*/

		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
		//	//glm::value_ptr(trans)
		//	amat.data()
		//);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}