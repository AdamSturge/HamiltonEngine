#pragma once

#include "Physics/State/RigidBodyState.h"

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	float ComputeConstantGravityPotential(const Eigen::Vector3f& Pos,
		float Mass);

	void ComputeGradConstantGravityPotential(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy);

	float ComputeConstantGravityPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor);

	void ComputeGradConstantGravityPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor,
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy);

	struct RigidBodyGravityComponent : public RigidBodyPotentialEnergyListComponent
	{
		RigidBodyGravityComponent(entt::const_handle Parent);

		float Gravity;
	};
}