#include "PrecompiledHeader/Pch.h"

#include "ConstantGravityPotential.h"
#include "Physics/State/ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<float> GravitationalAcceleration("GravitationalAcceleration", 9.8f);
	
	float ComputeConstantGravityPotential(const Eigen::Vector3f& Pos,
		float Mass)
	{
		const float Height = Pos.z();
		return Mass * GravitationalAcceleration * Height;
	}

	void ComputeGradConstantGravityPotential(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		OutGradPotentialEnergy = Mass * GravitationalAcceleration * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	}
}