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
	void RigidBodyPotentialOnly(float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom,
		float PotentialWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void RigidBodyKineticOnly(float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);
}