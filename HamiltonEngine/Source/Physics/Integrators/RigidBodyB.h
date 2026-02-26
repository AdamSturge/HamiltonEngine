#pragma once

#include "Configuration/Globals.h"

/*
	RigidBodyB is a Rigid Body integration technique based on EulerB

	See RigidBodyA for more details
*/

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	void RigidBodyB(float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		entt::const_handle PotentialEnergyEntity);
}