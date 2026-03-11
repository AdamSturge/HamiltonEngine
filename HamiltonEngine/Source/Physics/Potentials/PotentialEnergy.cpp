#include "PrecompiledHeader/Pch.h"

#include "PotentialEnergy.h"
#include "Configuration/Globals.h"
#include "ConstantGravityPotential.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
	float ComputePotentialEnergyParticle(const Eigen::Vector3f& Position,
		float Mass,
		entt::const_handle PotentialEnergyEntity)
	{
		float PotentialEnergy = 0.0f;

		entt::const_handle CurrentEntityHandle = PotentialEnergyEntity;
		const entt::registry& Reigstry = HamiltonEngine::Globals::Registry;
		while (CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?

			if (const ParticleGravityComponent* GravityComponent = CurrentEntityHandle.try_get<ParticleGravityComponent>())
			{
				PotentialEnergy += ComputeConstantGravityPotentialParticle(Position, Mass);
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Computing particle potential energy and encountered unknown energy type. Ending iteration on entity %d:%d", 
					EntityId,
					EntityVersion)

				break;
			}
		}

		return PotentialEnergy;
	}

	void ComputeGradPotentialEnergyParticle(const Eigen::Vector3f& Position,
		float Mass,
		entt::const_handle PotentialEnergyEntity,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		entt::const_handle CurrentEntityHandle = PotentialEnergyEntity;
		const entt::registry& Reigstry = HamiltonEngine::Globals::Registry;
		while (CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?

			if (const ParticleGravityComponent* GravityComponent = CurrentEntityHandle.try_get<ParticleGravityComponent>())
			{
				ComputeGradConstantGravityPotentialParticle(Mass, OutGradPotentialEnergy);
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Computing particle potential energy gradient and encountered unknown energy type. Ending iteration on entity %d:%d", 
					EntityId, 
					EntityVersion)
					
					break;
			}

		}
		
	}

	void ComputeGradPotentialEnergyRigidBody(const Eigen::Affine3f& Transform,
		float Mass,
		Eigen::Diagonal3f InertiaTensor,
		entt::const_handle PotentialEnergyEntity,
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{	
		const Eigen::Vector3f WorldPosition = Transform.translation();
		const Eigen::Matrix3f Orientation = Transform.rotation();
		
		entt::const_handle CurrentEntityHandle = PotentialEnergyEntity;
		const entt::registry& Reigstry = HamiltonEngine::Globals::Registry;
		while(CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?
			
			if (const RigidBodyGravityComponent* GravityComponent = CurrentEntityHandle.try_get<RigidBodyGravityComponent>())
			{
				const Eigen::Vector3f BodyPointOfApplication = Eigen::Vector3f::Zero();
				ComputeGradConstantGravityPotentialRigidBody(Transform,
					BodyPointOfApplication,
					Mass,
					InertiaTensor,
					OutGradLinearPotentialEnergy,
					OutGradAngularPotentialEnergy);
				
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else 
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Computing rigid body potential energy gradient and encountered unknown energy type. Ending iteration on entity %d:%d",
					EntityId,
					EntityVersion)

				break;
			}

		} 
	}
}
