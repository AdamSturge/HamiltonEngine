#pragma once
#include <entt/entt.hpp>
#include <Eigen/Dense>

namespace HamiltonEngine::Physics 
{
	struct PositionComponent 
	{
		PositionComponent(Eigen::Vector3f Pos);
		Eigen::Vector3f Position;
	};

	struct LinearMomentumComponent
	{
		LinearMomentumComponent(Eigen::Vector3f Mom);
		Eigen::Vector3f LinearMomentum;
	};

	struct MassComponent
	{
		MassComponent(float M);
		float Mass;
	};
}