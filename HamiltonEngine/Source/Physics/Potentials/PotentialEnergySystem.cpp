#include "PrecompiledHeader/Pch.h"

#include "PotentialEnergySystem.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "ConstantGravityPotentialSystem.h"

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
}
