
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
}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json");

	entt::registry registry;
	HamiltonEngine::CreateEntities(registry);

	return 0;
}
