#pragma once

#include "Configuration/Globals.h"

/*
	For a free rigid body (ie only kinetic contributions to the hamiltonian)
	the differential equation describing the evolution of the angular momentum b, in body coordinates, is:
	db/dt = b x (I^-1) b 
	where I is the inertia tensor.
	If we consider only the X part of the angular momentum this cross product can be written
	dl/dt = A_x b 
	for a special matrix A_x. This can be explicitly solved for b(t)
	b(t) = exp(A_x t) b(0)
	Similar matricies can be found for Y and Z.

	Writing the columns of the orientation matrix as s_x,s_y_,s_z we can apply the same treatment to the position upate
	ds/dt = s x (I^-1) b
	The solution to this differential equation is the same as above
	s(t) = exp(A_x t) s(0).

	Thus we can compose 3 steps, one for X,Y,and Z, to fully integrate the kinetic part of the hamiltonian

	Moving on to the Potential portion of the update we must now consider the effects of a potential
	on a particular portion of the rigid body

	TODO finish this once I have a better idea of how it works

	RigidBodyA is a Rigid Body integration technique based on EulerA with the folowing update rule given a Hamiltonian
	H = T + V
*/

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	void RigidBodyA(float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		entt::const_handle PotentialEnergyEntity);
}