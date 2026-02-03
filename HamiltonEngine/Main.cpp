
#include "Physics/PhysicsComponent.h"

#include "entt/entt.hpp"
#include <Eigen/Dense>

namespace HamiltonEngine
{
	void CreateEntities(entt::registry& registry)
	{
		entt::entity entity = registry.create();
		registry.emplace<Physics::PhysicsState>(entity);
	}
}

int main(int argc, char** argv)
{
	entt::registry registry;
	HamiltonEngine::CreateEntities(registry);

	return 0;
}
