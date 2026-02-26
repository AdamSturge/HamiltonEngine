#pragma once

#include "Configuration/Globals.h"

/*
	EulerB is a version of symplectic Euler with the folowing update rule given a Hamiltonian
	H = T + V
	p_{n+1} = p_{n} - dt*GradV(q_n) <-- dp/dt = -dH/dq = -GradV(q)
	q_{n+1} = q_{n} + dt+p_{n+1}    <-- dq/dt = -dH/dp = dT/dp = p
*/

namespace HamiltonEngine::Physics 
{
	void EulerB(const float Mass,
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		entt::const_handle PotentialEnergyEntity,
		float Dt = Globals::PhysicsTickLength);
}