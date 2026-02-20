#pragma once

#include "Configuration/Globals.h"

/*
	StormerVerlet has the folowing update rule:
	Hamiltonian H = 0.5V + T + 0.5V
	p_{n+0.5} = p_{n} - 0.5f*dt*GradV(q_n)
	q_{n+1} = q_{n} + dt+p_{n+0.5}
	p_{n+1} = p_{n+0.5} - 0.5f*dt*GradV(q_{n+1})
*/

namespace HamiltonEngine::Physics
{
	void StormerVerlet(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt = Globals::PhysicsTickLength);
}