#pragma once

#include "EulerSystem.h"
#include "Configuration/Globals.h"

#include <utility>

namespace HamiltonEngine::Physics 
{
	struct MassComponent;
	struct PositionComponent;
	struct LinearMomentumComponent;

	using EulerModePair = std::pair<EulerMode, EulerMode>;

	//Split the Hamiltonian H = T + V to produce higher order methods
	//A are the weights for the potnential energy split. H = a0V + a1*V (w0 + w1 = 1)
	//B are the weights for the kinentic energy split  H = b0T + b1T (b0 + b1 = 1)
	//C are the weights for the potnential energy dT substepping Phi_{V,dt} = Phi_{V,c1*dt} o Phi_{V,c0*dt} (c0 + c1 = 1)
	//D are the weights for the kinetic energy dT substepping Phi_{V,dt} = Phi_{T,d1*dt} o Phi_{T,d0*dt} (d0 + d1 = 1)
	//Each splitting step runs 2 steps of either potential or kinematic integration
	template<int N>
	void HamiltonianSplittingSystem(const float A[N], 
		const float B[N],
		const float C[N], 
		const float D[N],
		const EulerModePair Modes[N], // 2 modes per step
		const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		float Dt = Globals::PhysicsTickLength)
	{
		for (int Index = 0; Index < N; ++Index) 
		{
			const float PotentialDt = C[Index] * Dt;
			const float KineticDt = D[Index] * Dt;
			
			switch (Modes[Index].first) 
			{
				case EulerMode::PotentialOnly: 
				{
					EulerSystem<EulerMode::PotentialOnly>(MassC, 
						PosC, 
						LinMomC, 
						A[Index], 
						B[Index], 
						PotentialDt);
					break;
				}
				case EulerMode::KineticOnly:
				{
					EulerSystem<EulerMode::KineticOnly>(MassC,
						PosC, 
						LinMomC,
						A[Index],
						B[Index],
						KineticDt);
					break;
				}
				default: 
				{
					//LOG this
				}
			}

			switch (Modes[Index].second)
			{
				case EulerMode::PotentialOnly:
				{
					EulerSystem<EulerMode::PotentialOnly>(MassC,
						PosC, 
						LinMomC,
						A[Index],
						B[Index],
						PotentialDt);
					break;
				}
				case EulerMode::KineticOnly:
				{
					EulerSystem<EulerMode::KineticOnly>(MassC,
						PosC,
						LinMomC,
						A[Index],
						B[Index],
						KineticDt);
					break;
				}
				default:
				{
					//LOG this
				}
			}

		}
	}
}