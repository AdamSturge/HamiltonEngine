#pragma once
#include <entt/entt.hpp>
#include <Eigen/Dense>

namespace HamiltonEngine::Physics 
{
	struct Position 
	{
		Eigen::Vector3d Position;
	};

	struct LinearMomentum
	{
		Eigen::Vector3d Position;
	};
}