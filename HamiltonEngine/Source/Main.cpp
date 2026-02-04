
#include "Configuration/ConfigurationSystem.h"
#include "Physics/PhysicsComponent.h"
#include "Configuration/ConfigurationVariable.h"


#include "entt/entt.hpp"
#include <glfw3.h>
#include <iostream>



namespace HamiltonEngine
{
	void CreateEntities(entt::registry& registry)
	{
		entt::entity entity = registry.create();
		registry.emplace<Physics::PhysicsComponent>(entity);
	}

	ConfigurationVariable<std::string> TestVar("TestKey", "Adam");
}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json");
	const HamiltonEngine::ConfigurationSystem& Config = HamiltonEngine::ConfigurationSystem::Get();

	std::cout << HamiltonEngine::TestVar << std::endl;

	entt::registry registry;
	HamiltonEngine::CreateEntities(registry);

	return 0;
}
