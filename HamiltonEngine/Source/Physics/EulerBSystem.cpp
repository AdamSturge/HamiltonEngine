#include "PrecompiledHeader/Pch.h"

#include "EulerBSystem.h"
#include "Configuration/Globals.h"
#include "PotentialEnergySystem.h"
#include "RigidBodyState.h"

namespace HamiltonEngine::Physics
{
	void EulerModeBPotentialOnlySystem(const MassComponent& MassC, 
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC)
	{
		const TransformComponent TransformC = PositionComponentToTransformComponent(PosC);	
		PotentialEnergyGradient PotentialEnergyGradient;
		GradPotentialEnergySystem(TransformC, MassC, PotentialEnergyGradient);
		
		LinMomC.LinearMomentum -= Globals::PhysicsTickLength * PotentialEnergyGradient;
	}

	void EulerModeBKineticOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC)
	{
		PosC.Position += Globals::PhysicsTickLength * LinMomC.LinearMomentum;
	}
}