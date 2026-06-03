#include "PrecompiledHeader/Pch.h"

#include "ConstantGravityPotential.h"
#include "Physics/State/ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<float> GravitationalAcceleration("GravitationalAcceleration", 9.8f);
	
	float ComputeConstantGravityPotentialParticle(const Eigen::Vector3f& Position,
		float Mass)
	{
		const float Height = Position.z();
		return Mass * GravitationalAcceleration * Height;
	}

	void ComputeGradConstantGravityPotentialParticle(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		OutGradPotentialEnergy += Mass * GravitationalAcceleration * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	}

	float ComputeConstantGravityPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition,
		float Mass)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		const float Height = WorldPosition.z();
		return Mass * GravitationalAcceleration * Height;
	}

	void ComputeGradConstantGravityPotentialRigidBody(float Mass,
		Eigen::Vector3f& OutGradLinearPotentialEnergy)
	{
		OutGradLinearPotentialEnergy += Mass * GravitationalAcceleration * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	}

	SERIALIZATION_DEFINITION_SAVE_COMPONENT_DEFAULT(HamiltonEngine::Physics::RigidBodyGravityComponent)
	{
		Record(cereal::make_nvp("NextEntity", Component.NextEntity.entity()));
		Record(cereal::make_nvp("RigidBodyEntity", Component.RigidBodyEntity.entity()));
		
		Record(cereal::make_nvp("Gravity", Component.Gravity));
	}

	SERIALIZATION_DEFINITION_LOAD_COMPONENT_DEFAULT(HamiltonEngine::Physics::RigidBodyGravityComponent)
	{
		entt::entity NextEntity{};
		Record(cereal::make_nvp("NextEntity", NextEntity));
		Component.NextEntity = entt::const_handle(Globals::Registry, NextEntity);

		entt::entity RigidBodyEntity{};
		Record(cereal::make_nvp("RigidBodyEntity", RigidBodyEntity));
		Component.RigidBodyEntity = entt::const_handle(Globals::Registry, RigidBodyEntity);
		
		Record(cereal::make_nvp("Gravity", Component.Gravity));
	}

	ParticleGravityComponent::ParticleGravityComponent(entt::const_handle Parent)
		: ParticleEntity{ Parent }
		, Gravity{ GravitationalAcceleration }
	{
	}

	RigidBodyGravityComponent::RigidBodyGravityComponent()
	{
		Gravity = 9.8f;
	}

	RigidBodyGravityComponent::RigidBodyGravityComponent(entt::const_handle Parent)
		: RigidBodyEntity{ Parent }
		, Gravity{ GravitationalAcceleration }
	{

	}
}