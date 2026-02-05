
#include "Configuration/ConfigurationSystem.h"
#include "Physics/PhysicsComponent.h"
#include "Configuration/ConfigurationVariable.h"

#include "entt/entt.hpp"
#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <iostream>
#include <OpenGL/Window.h>
namespace HamiltonEngine
{
	//ConfigurationVariable<float> PhysicsTickLength("PhysicsTickLength", 1.0 / 60.0f);
	
	void CreateEntities(entt::registry& registry)
	{
		entt::entity entity = registry.create();
		registry.emplace<Physics::Position>(entity);
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

	while (!glfwWindowShouldClose(window)) {

		// input
		HamiltonEngine::OpenGL::processInput(window);

		// rendering
		//std::this_thread::sleep_for(20ms);

		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
