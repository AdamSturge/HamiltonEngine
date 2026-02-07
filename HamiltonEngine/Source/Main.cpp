
#include "Configuration/ConfigurationSystem.h"
#include "Physics/PhysicsComponent.h"
#include "Configuration/ConfigurationVariable.h"
#include "Physics/SymplecticEulerSystem.h"

#include "entt/entt.hpp"
#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <iostream>
#include <OpenGL/Window.h>
namespace HamiltonEngine
{
	void CreatePhysicsEntities(entt::registry& Registry)
	{
		entt::entity Entity = Registry.create();
		Registry.emplace<Physics::PositionComponent>(Entity, Eigen::Vector3f::Zero());
		Registry.emplace<Physics::LinearMomentumComponent>(Entity, Eigen::Vector3f(1.0f,0.0f,0.0f));
		Registry.emplace<Physics::MassComponent>(Entity, 1.0f);
	}
}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json");

	entt::registry Registry;
	HamiltonEngine::CreatePhysicsEntities(Registry);

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

		// Run Physics Sim
		auto PhysicsSimView = Registry.view<
			HamiltonEngine::Physics::PositionComponent,
			HamiltonEngine::Physics::LinearMomentumComponent>();

		for (auto [Entity, PosC, LinMomC] : PhysicsSimView.each())
		{
			HamiltonEngine::Physics::SymplecticEulerSystem(PosC, LinMomC);
		}

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
