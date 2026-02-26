#pragma once

#include "Configuration/Globals.h"

/*
	Writing H = H1 + H2 (H1 = T, H2 = V) we can split this update in two via a splitting/composition of flow method
	These functions are used to build higher order methods via the splitting technique.
	See EulerB
*/

namespace HamiltonEngine::Physics 
{
	enum class EulerIntegrationCompositionMode
	{
		Kinetic,
		Potential
	};
	
	void EulerPotentialOnly(float Mass,
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		entt::const_handle PotentialEnergyEntity,
		float PotentialWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void EulerKineticOnly(float Mass,
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);
}