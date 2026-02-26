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

	struct ParticlePotentialEnergyListComponent
	{
		ParticlePotentialEnergyListComponent(entt::const_handle Parent);

		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given particle
		entt::const_handle NextEntity;
		entt::const_handle ParticleEntity; //back pointer to particle
	};
}