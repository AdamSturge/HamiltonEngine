#pragma once

#include "Configuration/Globals.h"

/*
	Writing H = H1 + H2 (H1 = T, H2 = V) we can split this update in two via a splitting/composition of flow method
	These functions are used to build higher order methods via the splitting technique.
*/
namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	enum class RigidBodyIntegrationCompositionMode 
	{
		KineticX,
		KineticY,
		KineticZ,
		Potential
	};

	struct RigidBodyPotentialEnergyComponent;
	
	void RigidBodyPotentialOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		const RigidBodyPotentialEnergyComponent& PotentialEnergyComponent,
		float PotentialWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void RigidBodyKineticXOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void RigidBodyKineticYOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void RigidBodyKineticZOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);
}