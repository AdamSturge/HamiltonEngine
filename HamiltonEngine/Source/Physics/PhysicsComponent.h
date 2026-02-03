#pragma once
#include <entt/entt.hpp>
#include <Eigen/Dense>

namespace HamiltonEngine::Physics 
{
	struct PhysicsComponent 
	{
		Eigen::Vector3d Position;
		Eigen::Vector3d LinearMomentum;
	};
}