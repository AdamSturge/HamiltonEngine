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


	//TODO This still needs time to cook. The output here should be torque (r x gradV)
	//so we can't just sum all all the gradV terms since they are applied at different
	//locations. However we do want to sum up gradV terms for the particle update system 
	// above
	void ComputeGradPotentialEnergy(const Eigen::Affine3f& Transform,
		const RigidBodyPotentialEnergyComponent& PotentialEnergyComponent, 
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{	
		const Eigen::Vector3f WorldPosition = Transform.translation();
		const Eigen::Matrix3f Orientation = Transform.rotation();
		
		const RigidBodyPotentialEnergyComponent* Current = &PotentialEnergyComponent;
		entt::entity NextEntity;
		const entt::registry& Reigstry = HamiltonEngine::Globals::Registry;
		do 
		{
			if (Current->ComputePotentialEnergyGradFn)
			{
				Current->ComputePotentialEnergyGradFn(Transform, 
					Current->BodyPointOfApplication,
					OutGradLinearPotentialEnergy,
					OutGradAngularPotentialEnergy);
			}

			NextEntity = Current->NextEntity;
			Current = Reigstry.try_get<RigidBodyPotentialEnergyComponent>(NextEntity);

		} while (Current);
	}
}
