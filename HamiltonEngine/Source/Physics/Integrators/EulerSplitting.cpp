#include "PrecompiledHeader/Pch.h"

#include "EulerSplitting.h"
#include "Configuration/Globals.h"
#include "Physics/Potentials/PotentialEnergy.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Physics
{
	void EulerPotentialOnly(float Mass, 
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		entt::const_handle PotentialEnergyEntity,
		float PotentialWeight,
		float Dt)
	{	
		Eigen::Vector3f PotentialEnergyGradient = Eigen::Vector3f::Zero();
		ComputeGradPotentialEnergyParticle(Position,
			Mass, 
			PotentialEnergyEntity,
			PotentialEnergyGradient);
		
		LinearMomentum -= Dt * PotentialWeight * PotentialEnergyGradient;
	}

	void EulerKineticOnly(const float Mass,
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		float KineticWeight,
		float Dt)
	{
		Position += Dt * KineticWeight * LinearMomentum;
	}
}