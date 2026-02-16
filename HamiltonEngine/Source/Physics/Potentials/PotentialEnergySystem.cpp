#include "PrecompiledHeader/Pch.h"

#include "PotentialEnergySystem.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "ConstantGravityPotentialSystem.h"

namespace HamiltonEngine::Physics 
{
	float PotentialEnergySystem(const TransformComponent& TransformC, 
		const MassComponent& MassC)
	{
		float PotentialEnergy = 0.0f;

		const PositionComponent PositionC = TransformComponentToPositionComponent(TransformC);
		PotentialEnergy = ConstantGravityPotentialSystem(PositionC, MassC);
		
		return PotentialEnergy;
	}

	void GradPotentialEnergySystem(const TransformComponent& TransformC,
		const MassComponent& MassC,
		PotentialEnergyGradient& OutGradPotentialEnergy)
	{
		const PositionComponent PositionC = TransformComponentToPositionComponent(TransformC);

		GradConstantGravityPotentialSystem(PositionC, MassC, OutGradPotentialEnergy);
	}
}
