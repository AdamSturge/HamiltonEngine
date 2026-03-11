#pragma once

#include "Physics/State/RigidBodyState.h"

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	float ComputeSprintPotential(const Eigen::Vector3f& Position,
		float Mass);

	void ComputeGradSpringPotential(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy);

	float ComputeSpringPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor);

	void ComputeGradSpringPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor,
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy);

	struct ParticleSpringComponent
	{
		ParticleSpringComponent(entt::const_handle Parent);

		float Gravity;
	};

	struct RigidBodySpringComponent
	{
		RigidBodySpringComponent(entt::const_handle Parent);

		float Gravity;
	};
}