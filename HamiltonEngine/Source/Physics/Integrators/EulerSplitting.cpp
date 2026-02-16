#include "PrecompiledHeader/Pch.h"

#include "EulerSplitting.h"
#include "Configuration/Globals.h"
#include "Physics/Potentials/PotentialEnergy.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Physics
{
	void EulerPotentialOnly(float Mass, 
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float PotentialWeight,
		float Dt)
	{
		Eigen::Affine3f Transform;
		Transform.translate(Pos);
		
		Eigen::Vector3f PotentialEnergyGradient;
		ComputeGradPotentialEnergy(Transform, Mass, PotentialEnergyGradient);
		
		LinMom -= Dt * PotentialWeight * PotentialEnergyGradient;
	}

	void EulerKineticOnly(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float KineticWeight,
		float Dt)
	{
		Pos += Dt * KineticWeight * LinMom;
	}
}