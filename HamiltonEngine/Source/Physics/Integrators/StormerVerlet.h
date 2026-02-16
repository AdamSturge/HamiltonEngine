#pragma once

#include "Configuration/Globals.h"

/*
	StormerVerlet is a version of symplectic Euler with the folowing update rule given a Hamiltonian
	H = 0.5V + T + 0.5V
	p_{n+1} = p_{n} - dt*GradV(q_n) <-- dp/dt = -dH/dq = -GradV(q)
	q_{n+1} = q_{n} + dt+p_{n+1}    <-- dq/dt = -dH/dp = dT/dp = p
*/

namespace HamiltonEngine::Physics
{
	void StormerVerlet(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt = Globals::PhysicsTickLength);
}