#pragma once

#include "EulerSplitting.h"
#include "Configuration/Globals.h"

#include <utility>

namespace HamiltonEngine::Physics 
{
	struct MassComponent;
	struct PositionComponent;
	struct LinearMomentumComponent;

	//using EulerModePair = std::pair<EulerMode, EulerMode>;

	//Split the Hamiltonian H = T + V to produce higher order methods
	//A are the weights for the potnential energy split. H = a0V + a1*V (w0 + w1 = 1)
	//B are the weights for the kinentic energy split  H = b0T + b1T (b0 + b1 = 1)
	//C are the weights for the potnential energy dT substepping Phi_{V,dt} = Phi_{V,c1*dt} o Phi_{V,c0*dt} (c0 + c1 = 1)
	//D are the weights for the kinetic energy dT substepping Phi_{V,dt} = Phi_{T,d1*dt} o Phi_{T,d0*dt} (d0 + d1 = 1)
	//Each splitting step runs 2 steps of either potential or kinematic integration
	template<int N, int M>
	void HamiltonianSplitting(const float A[N], 
		const float B[M],
		const float C[N], 
		const float D[M],
		const EulerMode Modes[N + M],
		const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt = Globals::PhysicsTickLength)
	{
		const int NumSteps = N + M;
		for (int Index = 0; Index < NumSteps; ++Index)
		{
			switch (Modes[Index]) 
			{
				case EulerMode::PotentialOnly: 
				{
					//Should never wrap but do mod just to avoid crashing in
					//case of bad inputs
					const int ModIndex = Index % N;
					const float PotentialDt = C[ModIndex] * Dt;
					EulerPotentialOnly(Mass,
						Pos, 
						LinMom, 
						A[ModIndex],
						PotentialDt);
					break;
				}
				case EulerMode::KineticOnly:
				{
					const int ModIndex = Index % M;
					const float KineticDt = D[ModIndex] * Dt;
					EulerKineticOnly(Mass,
						Pos, 
						LinMom,
						B[ModIndex],
						KineticDt);
					break;
				}
				default: 
				{
					//LOG this
				}//LOG this
			}

		}
	}
}