#pragma once

#include "Configuration/Globals.h"

/*
	EulerA is a version of symplectic Euler with the folowing update rule given a Hamiltonian
	H = T + V
	q_{n+1} = q_{n} + dt+p_{n}    <-- dq/dt = -dH/dp = dT/dp = p
	p_{n+1} = p_{n} - dt*GradV(q_{n+1}) <-- dp/dt = -dH/dq = -GradV(q)
*/

namespace HamiltonEngine::Physics
{
	void EulerA(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt = Globals::PhysicsTickLength);
}