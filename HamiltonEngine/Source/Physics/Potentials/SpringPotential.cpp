#include "PrecompiledHeader/Pch.h"

#include "SpringPotential.h"

namespace HamiltonEngine::Physics
{
	float ComputeSpringPotentialParticle(const Eigen::Vector3f& Position,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength)
	{
		const Eigen::Vector3f Diff = Position - OtherEndOfSpringPosition;
		return 0.5f * SpringConstant * std::powf(Diff.norm() - RestLength, 2);
	}

	void ComputeGradSpringPotentialParticle(const Eigen::Vector3f& Position,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		Eigen::Vector3f Direction = Position - OtherEndOfSpringPosition;
		const float Magnitude = Direction.norm() - RestLength;
		Direction.normalize();
		OutGradPotentialEnergy += SpringConstant * Magnitude * Direction;
	}

	float ComputeSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, 
		const Eigen::Vector3f& OtherEndOfSpringPosition, float SpringConstant, float RestLength, const Eigen::Diagonal3f& InertiaTensor)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		const Eigen::Vector3f Diff =  WorldPosition - OtherEndOfSpringPosition;
		return 0.5f * SpringConstant * std::powf(Diff.norm() - RestLength, 2);
	}

	void ComputeGradSpringPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition, const Eigen::Vector3f& OtherEndOfSpringPosition
		,float SpringConstant, float RestLength, const Eigen::Diagonal3f& InertiaTensor, Eigen::Vector3f& OutGradLinearPotentialEnergy, Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		Eigen::Vector3f Direction = WorldPosition - OtherEndOfSpringPosition;
		const float Magnitude = Direction.norm() - RestLength;
		Direction.normalize();

		OutGradLinearPotentialEnergy += SpringConstant * Magnitude * Direction;

		//Angular quantities are in body coordinates
		const auto InvRotation = BodyToWorldTransform.rotation().transpose();
		//Minus sign due to F = -GradV
		OutGradAngularPotentialEnergy -= BodyPosition.cross(InvRotation * OutGradLinearPotentialEnergy);
	}

	SpringPotentialComponent::SpringPotentialComponent()
		: K{1.0f}
		, L{0.0f}
		, AnchorPointBody{0.0f,0.0f,0.0f}
		, Enabled{false}
	{

	}

	SpringPotentialComponent::SpringPotentialComponent(entt::const_handle Parent, float SpringConstant,
		float RestLength, const Eigen::Vector3f& BAnchorPoint, bool IsEnabled)
		: ParentEntity{ Parent }
		, K{ SpringConstant }
		, L{ RestLength }
		, AnchorPointBody{ BAnchorPoint }
		, Enabled{ IsEnabled }
	{
	}

	SpringPotentialComponent::SpringPotentialComponent(entt::const_handle Parent, float SpringConstant,
		float RestLength, const Eigen::Vector3f& BAnchorPoint)
		: SpringPotentialComponent(Parent, SpringConstant, RestLength, BAnchorPoint, true)

	{
	}

	SERIALIZATION_DEFINITION_SAVE_COMPONENT_DEFAULT(HamiltonEngine::Physics::SpringPotentialComponent)
	{
		Record(cereal::make_nvp("NextEntity", Component.NextEntity.entity()));
		Record(cereal::make_nvp("ParentEntity", Component.ParentEntity.entity()));

		Record(cereal::make_nvp("SpringConstant", Component.K));
		Record(cereal::make_nvp("RestLength", Component.L));
		Record(cereal::make_nvp("AnchorPointBody", Component.AnchorPointBody));
		Record(cereal::make_nvp("OtherEntity", Component.OtherEntity.entity()));
		Record(cereal::make_nvp("Enabled", Component.Enabled));
	}

	SERIALIZATION_DEFINITION_LOAD_COMPONENT_DEFAULT(HamiltonEngine::Physics::SpringPotentialComponent)
	{
		entt::entity NextEntity{};
		Record(cereal::make_nvp("NextEntity", NextEntity));
		Component.NextEntity = entt::const_handle(Globals::Registry, NextEntity);

		entt::entity ParentEntity{};
		Record(cereal::make_nvp("ParentEntity", ParentEntity));
		Component.ParentEntity = entt::const_handle(Globals::Registry, ParentEntity);

		Record(cereal::make_nvp("SpringConstant", Component.K));
		Record(cereal::make_nvp("RestLength", Component.L));
		Record(cereal::make_nvp("AnchorPointBody", Component.AnchorPointBody));

		entt::entity OtherEntity{};
		Record(cereal::make_nvp("OtherEntity", OtherEntity));
		Component.OtherEntity = entt::const_handle(Globals::Registry, OtherEntity);

		Record(cereal::make_nvp("Enabled", Component.Enabled));
	}
}