#pragma once

#include "ParticleState.h"

namespace Eigen 
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	struct RigidBodyStateComponent
	{
		RigidBodyStateComponent() = default;
		
		RigidBodyStateComponent(const Eigen::Affine3f& Trans,
			float M,
			const Eigen::Vector3f& LinearMomentum,
			const Eigen::Diagonal3f& I,
			const Eigen::Vector3f& AngMom);
		
		Eigen::Affine3f Transform; //Body To World
		
		//Linear state
		float Mass;
		Eigen::Vector3f LinearMomentum; //World coordinates
		
		//Angular State
		Eigen::Diagonal3f InertiaTensor; // Body Coordinates
		Eigen::Vector3f AngularMomentum; // Body Coordinates

		//Potential Energy
		entt::const_handle PotentialEnergyListHead;
	};
	
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version);
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version);
}

CEREAL_CLASS_VERSION(HamiltonEngine::Physics::RigidBodyStateComponent, 1);

