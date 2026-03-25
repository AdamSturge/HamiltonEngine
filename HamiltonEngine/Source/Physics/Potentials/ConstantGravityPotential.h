#pragma once

#include "Physics/State/RigidBodyState.h"

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	float ComputeConstantGravityPotentialParticle(const Eigen::Vector3f& Position,
		float Mass);

	void ComputeGradConstantGravityPotentialParticle(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy);

	float ComputeConstantGravityPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float Mass);

	void ComputeGradConstantGravityPotentialRigidBody(
		float Mass,
		Eigen::Vector3f& OutGradLinearPotentialEnergy);

	struct ParticleGravityComponent
	{
		ParticleGravityComponent(entt::const_handle Parent);

		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given particle
		entt::const_handle NextEntity;
		entt::const_handle ParticleEntity; //back pointer to particle

		float Gravity;
	};

	struct RigidBodyGravityComponent
	{
		RigidBodyGravityComponent(entt::const_handle Parent);
		
		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given particle
		entt::const_handle NextEntity;
		entt::const_handle RigidBodyEntity; //back pointer to rigid body
		
		float Gravity;
	};
}