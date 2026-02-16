#include "PrecompiledHeader/Pch.h"

#include "EulerSplitting.h"
#include "Configuration/Globals.h"
#include "Physics/Potentials/PotentialEnergySystem.h"
#include "Physics/State/RigidBodyState.h"

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