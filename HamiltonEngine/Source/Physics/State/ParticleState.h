#pragma once

namespace HamiltonEngine::Physics 
{
	struct ParticleStateComponent 
	{
		ParticleStateComponent(float M, 
			const Eigen::Vector3f& Position,
			const Eigen::Vector3f& LinearMomentum);

		float Mass; 
		Eigen::Vector3f Position;
		Eigen::Vector3f LinearMomentum;

		//Potential Energy
		entt::const_handle PotentialEnergyListHead;
	};
}