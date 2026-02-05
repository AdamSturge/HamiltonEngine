
#include "Configuration/ConfigurationSystem.h"
#include "Physics/PhysicsComponent.h"
#include "Configuration/ConfigurationVariable.h"

#include "entt/entt.hpp"
#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <iostream>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include <math.h>

namespace HamiltonEngine
{
	void CreateEntities(entt::registry& registry)
	{
		entt::entity entity = registry.create();
		registry.emplace<Physics::PhysicsComponent>(entity);
	}
}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json");

	entt::registry registry;
	HamiltonEngine::CreateEntities(registry);

	glfwInit(); // Initialize OpenGL
	GLFWwindow* window = HamiltonEngine::OpenGL::createWindow(800, 600, "MyWindow");

	if (window == NULL)
	{
		return -1;
	}

	float red = 0.2f;
	float green = 0.3f;
	float blue = 0.3f;
	float curTime = 0.0f;

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VAO, VBO;

	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	HamiltonEngine::OpenGL::Shader simpleShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs", 
																						 "Source\\Shaders\\fragmentShader.fs");

	while (!glfwWindowShouldClose(window)) {

		// input
		HamiltonEngine::OpenGL::processInput(window);

		// rendering
		//std::this_thread::sleep_for(20ms);

		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		curTime = glfwGetTime() / 2;
		simpleShader.use();
		simpleShader.setFloat("hOffset", (float)sin(curTime));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}