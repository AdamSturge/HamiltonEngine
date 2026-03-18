#include "PrecompiledHeader/Pch.h"

#include "SpringPotential.h"

namespace HamiltonEngine::Physics
{
	float ComputeSpringPotentialParticle(const Eigen::Vector3f& Position,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength)
	{
		const Eigen::Vector3f Diff =  OtherEndOfSpringPosition - Position;
		return 0.5f * SpringConstant * std::pow(Diff.norm() - RestLength, 2);
	}

	void ComputeGradSpringPotentialParticle(const Eigen::Vector3f& Position,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		Eigen::Vector3f Direction = OtherEndOfSpringPosition - Position;
		const float Magnitude = Direction.norm() - RestLength;
		Direction.normalize();
		OutGradPotentialEnergy += SpringConstant * Magnitude * Direction;
	}

	float ComputeSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, 
		const Eigen::Vector3f& OtherEndOfSpringPosition, float SpringConstant, float RestLength, const Eigen::Diagonal3f& InertiaTensor)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		const Eigen::Vector3f Diff = OtherEndOfSpringPosition - WorldPosition;
		return 0.5f * SpringConstant * std::pow(Diff.norm() - RestLength, 2);
	}

	void ComputeGradSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, const Eigen::Vector3f& OtherEndOfSpringPosition
		,float SpringConstant, float RestLength, const Eigen::Diagonal3f& InertiaTensor, Eigen::Vector3f& OutGradLinearPotentialEnergy, Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		Eigen::Vector3f Direction = OtherEndOfSpringPosition - WorldPosition;
		const float Magnitude = Direction.norm() - RestLength;
		Direction.normalize();
		OutGradLinearPotentialEnergy += SpringConstant * Magnitude * Direction;

		//TODO angular potential should change
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