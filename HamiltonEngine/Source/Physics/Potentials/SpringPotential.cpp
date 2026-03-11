#include "PrecompiledHeader/Pch.h"

#include "SpringPotential.h"

namespace HamiltonEngine::Physics
{
	float ComputeSpringPotentialParticle(const Eigen::Vector3f& Position, 
		float SpringConstant,
		float RestLength)
	{
		
		return 0.0f;
	}

	void ComputeGradSpringPotentialParticle(float SpringConstant, Eigen::Vector3f& OutGradPotentialEnergy)
	{
	}

	float ComputeSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, float SpringConstant, const Eigen::Diagonal3f& InertiaTensor)
	{
		return 0.0f;
	}

	void ComputeGradSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, float SpringConstant, 
		const Eigen::Diagonal3f& InertiaTensor, Eigen::Vector3f& OutGradLinearPotentialEnergy, Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{
	}

	SpringPotentialComponent::SpringPotentialComponent(entt::const_handle Parent, float SpringConstant,
		float RestLength, const Eigen::Vector3f& BAnchorPoint)
		: ParentEntity{Parent}
		, K{SpringConstant}
		, L{RestLength}
		, AnchorPointBody{BAnchorPoint}
	{
	}
}