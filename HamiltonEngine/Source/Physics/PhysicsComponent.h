#pragma once
#include <entt/entt.hpp>
#include <Eigen/Dense>

namespace HamiltonEngine::Physics 
{
	struct PhysicsState 
	{
		Eigen::Vector3d Position;
		Eigen::Vector3d LinearMomentum;
	};
}