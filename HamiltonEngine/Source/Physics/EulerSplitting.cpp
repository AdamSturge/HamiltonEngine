#include "PrecompiledHeader/Pch.h"

#include "EulerSystem.h"
#include "Configuration/Globals.h"
#include "PotentialEnergySystem.h"
#include "RigidBodyState.h"

namespace HamiltonEngine::Physics
{
	void EulerPotentialOnlySystem(const MassComponent& MassC, 
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC, 
		float PotentialWeight,
		float Dt)
	{
		const TransformComponent TransformC = PositionComponentToTransformComponent(PosC);	
		PotentialEnergyGradient PotentialEnergyGradient;
		GradPotentialEnergySystem(TransformC, MassC, PotentialEnergyGradient);
		
		LinMomC.LinearMomentum -= Dt * PotentialWeight * PotentialEnergyGradient;
	}

	void EulerKineticOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		float KineticWeight,
		float Dt)
	{
		PosC.Position += Dt * KineticWeight * LinMomC.LinearMomentum;
	}
}