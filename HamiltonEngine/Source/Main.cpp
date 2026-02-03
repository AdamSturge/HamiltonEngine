
#include "Physics/PhysicsComponent.h"

#include "entt/entt.hpp"
#include <Eigen/Dense>
#include <glad/glad.h>

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
	entt::registry registry;
	HamiltonEngine::CreateEntities(registry);

	return 0;
}
