#include "PrecompiledHeader/Pch.h"

#include "ConstantGravityPotentialSystem.h"
#include "Physics/State/ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<float> GravitationalAcceleration("GravitationalAcceleration", 9.8f);
	
	float ConstantGravityPotentialSystem(const PositionComponent& PosC,
		const MassComponent& MassC)
	{
		const float Height = PosC.Position.z();
		return MassC.Mass * GravitationalAcceleration * Height;
	}

	void GradConstantGravityPotentialSystem(const PositionComponent& PosC,
		const MassComponent& MassC,
		PotentialEnergyGradient& OutGradPotentialEnergy)
	{
		OutGradPotentialEnergy = MassC.Mass * GravitationalAcceleration * PotentialEnergyGradient(0.0f, 0.0f, 1.0f);
	}
}