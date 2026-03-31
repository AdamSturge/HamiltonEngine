#include "PrecompiledHeader/Pch.h"

#include "PotentialEnergy.h"
#include "Configuration/Globals.h"
#include "ConstantGravityPotential.h"
#include "SpringPotential.h"
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
		while (CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?

			if (const ParticleGravityComponent* GravityComponent = CurrentEntityHandle.try_get<ParticleGravityComponent>())
			{
				PotentialEnergy += ComputeConstantGravityPotentialParticle(Position, Mass);
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else if (const SpringPotentialComponent* SpringComponent = CurrentEntityHandle.try_get<SpringPotentialComponent>())
			{
				CurrentEntityHandle = SpringComponent->NextEntity;
			}
			else
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Entity %d:%d. Computing particle potential energy and encountered unknown energy type. Ending iteration", 
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
		while (CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?

			if (const ParticleGravityComponent* GravityComponent = CurrentEntityHandle.try_get<ParticleGravityComponent>())
			{
				ComputeGradConstantGravityPotentialParticle(Mass, OutGradPotentialEnergy);
				
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else if (const SpringPotentialComponent* SpringComponent = CurrentEntityHandle.try_get<SpringPotentialComponent>())
			{
				if (!SpringComponent->OtherEntity.valid())
				{
					CurrentEntityHandle = SpringComponent->NextEntity;

					const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
					const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
					const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
					const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

					HAMILTON_LOG(Physics,
						Error,
						"Entity %d:%d. Computing particle potential energy gradient for spring and entity on other end of spring is invalid",
						EntityId,
						EntityVersion,
						OtherEntityId,
						OtherEntityVersion)

						continue;
				}
				
				if (const SpringPotentialComponent* OtherSpringComponent = SpringComponent->OtherEntity.try_get<SpringPotentialComponent>()) 
				{
					if (!OtherSpringComponent->ParentEntity.valid())
					{
						CurrentEntityHandle = SpringComponent->NextEntity;

						const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
						const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
						const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
						const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

						HAMILTON_LOG(Physics,
							Error,
							"Entity %d:%d. Computing particle potential energy gradient for spring and entity on other end of spring has no parent",
							EntityId,
							EntityVersion,
							OtherEntityId,
							OtherEntityVersion)

							continue;
					}

					Eigen::Vector3f OtherEndOfSpringPosition = Eigen::Vector3f::Zero();
					if (const ParticleStateComponent* ParticleState = OtherSpringComponent->ParentEntity.try_get<ParticleStateComponent>())
					{
						OtherEndOfSpringPosition = ParticleState->Position;
						ComputeGradSpringPotentialParticle(Position, OtherEndOfSpringPosition, SpringComponent->K,
							SpringComponent->L, OutGradPotentialEnergy);
					}
					else if (const RigidBodyStateComponent* RigidBodyState = OtherSpringComponent->ParentEntity.try_get<RigidBodyStateComponent>())
					{
						OtherEndOfSpringPosition = RigidBodyState->Transform * OtherSpringComponent->AnchorPointBody;
						ComputeGradSpringPotentialParticle(Position, OtherEndOfSpringPosition, SpringComponent->K,
							SpringComponent->L, OutGradPotentialEnergy);
					}
					else 
					{
						const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
						const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
						const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
						const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

						HAMILTON_LOG(Physics,
							Error,
							"Entity %d:%d. Computing particle potential energy gradient for spring and could not determine position of other end of spring",
							EntityId,
							EntityVersion,
							OtherEntityId,
							OtherEntityVersion)
					}
					
				}
				else 
				{
					const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
					const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
					const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
					const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);
					
					HAMILTON_LOG(Physics,
						Warning,
						"Entity %d:%d. Computing particle potential energy gradient for spring and entity %d:%d on other end does not have a spring component",
						EntityId,
						EntityVersion,
						OtherEntityId,
						OtherEntityVersion)
				}
				
				
				CurrentEntityHandle = SpringComponent->NextEntity;
			}
			else
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Entity %d:%d. Computing particle potential energy gradient and encountered unknown energy type. Ending iteration", 
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

		Eigen::Matrix3f RotationMatrix = Transform.rotation();
		Eigen::AngleAxisf AxisAngle = Eigen::AngleAxisf(RotationMatrix);
		
		const Eigen::Vector3f WorldPosition = Transform.translation();
		const Eigen::Matrix3f Orientation = Transform.rotation();
		
		entt::const_handle CurrentEntityHandle = PotentialEnergyEntity;
		while(CurrentEntityHandle.valid())
		{
			//You need to add to this list whenever a new type of potential is added to the engine/game
			//TODO maybe some kind of global type registry?
			
			if (const RigidBodyGravityComponent* GravityComponent = CurrentEntityHandle.try_get<RigidBodyGravityComponent>())
			{
				ComputeGradConstantGravityPotentialRigidBody(Mass, OutGradLinearPotentialEnergy);
				
				CurrentEntityHandle = GravityComponent->NextEntity;
			}
			else if (const SpringPotentialComponent* SpringComponent = CurrentEntityHandle.try_get<SpringPotentialComponent>())
			{
				if (!SpringComponent->Enabled) 
				{
					CurrentEntityHandle = SpringComponent->NextEntity;
					continue;
				}
				
				if (!SpringComponent->OtherEntity.valid()) 
				{
					CurrentEntityHandle = SpringComponent->NextEntity;

					const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
					const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
					const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
					const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

					HAMILTON_LOG(Physics,
						Error,
						"Entity %d:%d. Computing rigid body potential energy gradient for spring and entity on other end of spring is invalid",
						EntityId,
						EntityVersion,
						OtherEntityId,
						OtherEntityVersion)

					continue;
				}

				if (const SpringPotentialComponent* OtherSpringComponent = SpringComponent->OtherEntity.try_get<SpringPotentialComponent>())
				{
					if (!OtherSpringComponent->ParentEntity.valid())
					{
						CurrentEntityHandle = SpringComponent->NextEntity;

						const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
						const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
						const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
						const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

						HAMILTON_LOG(Physics,
							Error,
							"Entity %d:%d. Computing rigid body potential energy gradient for spring and entity on other end of spring has no parent",
							EntityId,
							EntityVersion,
							OtherEntityId,
							OtherEntityVersion)

							continue;
					}
					
					Eigen::Vector3f OtherEndOfSpringPosition = Eigen::Vector3f::Zero();
					if (const ParticleStateComponent* ParticleState = OtherSpringComponent->ParentEntity.try_get<ParticleStateComponent>())
					{
						OtherEndOfSpringPosition = ParticleState->Position;
						ComputeGradSpringPotentialRigidBody(Transform, OtherSpringComponent->AnchorPointBody, OtherEndOfSpringPosition,
							SpringComponent->K, SpringComponent->L, InertiaTensor, OutGradLinearPotentialEnergy, OutGradAngularPotentialEnergy);
					}
					else if (const RigidBodyStateComponent* RigidBodyState = OtherSpringComponent->ParentEntity.try_get<RigidBodyStateComponent>())
					{
						OtherEndOfSpringPosition = RigidBodyState->Transform * OtherSpringComponent->AnchorPointBody;
						ComputeGradSpringPotentialRigidBody(Transform, SpringComponent->AnchorPointBody, OtherEndOfSpringPosition,
							SpringComponent->K, SpringComponent->L, InertiaTensor, OutGradLinearPotentialEnergy, OutGradAngularPotentialEnergy);
					}
					else
					{
						const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
						const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
						const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
						const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

						HAMILTON_LOG(Physics,
							Error,
							"Entity %d:%d. Computing rigid body potential energy gradient for spring and could not determine position of other end of spring",
							EntityId,
							EntityVersion,
							OtherEntityId,
							OtherEntityVersion)
					}
				}
				else 
				{
					const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
					const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
					const auto OtherEntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(SpringComponent->OtherEntity);
					const auto OtherEntityVersion = ENTIY_HADNLE_TO_VERSION(SpringComponent->OtherEntity);

					HAMILTON_LOG(Physics,
						Warning,
						"Entity %d:%d. Computing rigid body potential energy gradient for spring and entity %d:%d on other end does not have a spring component",
						EntityId,
						EntityVersion,
						OtherEntityId,
						OtherEntityVersion)
				}
				
				CurrentEntityHandle = SpringComponent->NextEntity;
			}
			else
			{
				const auto EntityId = ENTIY_HADNLE_TO_UNDERLYING_TYPE(CurrentEntityHandle);
				const auto EntityVersion = ENTIY_HADNLE_TO_VERSION(CurrentEntityHandle);
				HAMILTON_LOG(Physics,
					Warning,
					"Entity %d:%d. Computing rigid body potential energy gradient and encountered unknown energy type. Ending iteration",
					EntityId,
					EntityVersion)

				break;
			}

		} 
	}
}
