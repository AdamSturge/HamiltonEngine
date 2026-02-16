#pragma once

#include "Configuration/Globals.h"

/*
	Writing H = H1 + H2 (H1 = T, H2 = V) we can split this update in two via a splitting/composition of flow method
	These functions are used to build higher order methods via this splitting technique.
	See EulerB
*/

namespace HamiltonEngine::Physics 
{
	//Change this to flags?
	enum class EulerMode
	{
		None,
		KineticOnly,
		PotentialOnly
	};

	struct MassComponent;
	struct PositionComponent;
	struct LinearMomentumComponent;

	void EulerPotentialOnly(float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float PotentialWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void EulerKineticOnly(float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);
}