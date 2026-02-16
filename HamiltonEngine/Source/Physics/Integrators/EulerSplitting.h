#pragma once

#include "Configuration/Globals.h"

/*
	EulerB is a version of symplectic Euler with the folowing update rule given a Hamiltonian
	H = T + V
	p_{n+1} = p_{n} - dt*GradV(q_n) <-- dp/dt = -dH/dq = -GradV(q)
	q_{n+1} = q_{n} + dt+p_{n+1}    <-- dq/dt = -dH/dp = dT/dp = p

	Writing H = H1 + H2 (H1 = T, H2 = V) we can split this update in two via a splitting/composition of flow method
	The first is H2 = V
	p_{n+1} = p_{n} - dt*GradV(q_n)
	q_{n+1} = q_{n}

	The second is H1 = T
	p_{n+1} = p_n
	q_{n+1} = q_{n} + dt+p_{n+1}
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

	void EulerPotentialOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		float PotentialWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);

	void EulerKineticOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		float KineticWeight = 1.0f,
		float Dt = Globals::PhysicsTickLength);
}