#include "PrecompiledHeader/Pch.h"

#include "PotentialEnergy.h"
#include "Configuration/Globals.h"
#include "ConstantGravityPotential.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
	float ComputePotentialEnergy(const Eigen::Affine3f& Transform,
		float Mass)
	{
		float PotentialEnergy = 0.0f;

		const Eigen::Vector3f Position = Transform.translation();
		PotentialEnergy = ComputeConstantGravityPotential(Position, Mass);
		
		return PotentialEnergy;
	}

	void ComputeGradPotentialEnergy(const Eigen::Affine3f& Transform,
		float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		ComputeGradConstantGravityPotential(Mass, OutGradPotentialEnergy);
	}

	void ComputeGradPotentialEnergy(const Eigen::Affine3f& Transform,
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
			}

			if (const RigidBodyPotentialEnergyListComponent* ListComponent = CurrentEntityHandle.try_get<RigidBodyPotentialEnergyListComponent>()) 
			{
				CurrentEntityHandle = ListComponent->NextEntity;
			}
			else 
			{
				//TODO Log this?
				break;
			}

		} 
	}
}
