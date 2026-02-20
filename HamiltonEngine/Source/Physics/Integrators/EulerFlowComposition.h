#pragma once

#include "EulerSplitting.h"
#include "Configuration/Globals.h"

namespace HamiltonEngine::Physics 
{
	//Split the Hamiltonian H = T + V to produce higher order methods
	//PotentialWeights are the weights for the potnential energy split. H = a0V + a1*V (w0 + w1 = 1)
	//KineticWeights are the weights for the kinentic energy split  H = b0T + b1T (b0 + b1 = 1)
	//PotentialTickRateWeights are the weights for the potnential energy dT substepping Phi_{V,dt} = Phi_{V,c1*dt} o Phi_{V,c0*dt} (c0 + c1 = 1)
	//KineticTickRateWeights are the weights for the kinetic energy dT substepping Phi_{V,dt} = Phi_{T,d1*dt} o Phi_{T,d0*dt} (d0 + d1 = 1)
	//This method uses compile time programming to compose the splitting steps
	template<int NumPotential, int NumKinetic, 
		int PotentialIndex = 0, int KineticIndex = 0, 
		EulerIntegrationCompositionMode First,
		EulerIntegrationCompositionMode... Rest>
	inline void EulerFlowComposition(const float* PotentialWeights,
		const float* KineticWeights,
		const float* PotentialTickRateWeights,
		const float* KineticTickRateWeights,
		float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt = Globals::PhysicsTickLength)
	{
		static_assert((1 + sizeof...(Rest)) == (NumPotential + NumKinetic - PotentialIndex - KineticIndex),
			"Length of composition mode list is incorrect. Please ensure your template arguments are correct");
		
		if constexpr (First == EulerIntegrationCompositionMode::Potential &&
			PotentialIndex >= 0 && PotentialIndex < NumPotential)
		{
			const float PotentialDt = PotentialTickRateWeights[PotentialIndex] * Dt;
			EulerPotentialOnly(Mass,
				Pos,
				LinMom,
				PotentialWeights[PotentialIndex],
				PotentialDt);

			constexpr int NextIndex = PotentialIndex + 1;
			if constexpr (NextIndex < NumPotential || KineticIndex < NumKinetic)
			{
				EulerFlowComposition<NumPotential, NumKinetic, NextIndex, KineticIndex, Rest...>(PotentialWeights,
					KineticWeights,
					PotentialTickRateWeights,
					KineticTickRateWeights,
					Mass,
					Pos,
					LinMom,
					Dt);
			}
		}

		if constexpr (First == EulerIntegrationCompositionMode::Kinetic 
			&& KineticIndex >= 0 && KineticIndex < NumKinetic)
		{
			const float KineticDt = KineticTickRateWeights[KineticIndex] * Dt;
			EulerKineticOnly(Mass,
				Pos,
				LinMom,
				KineticWeights[KineticIndex],
				KineticDt);

			constexpr int NextIndex = KineticIndex + 1;
			if constexpr (PotentialIndex < NumPotential || NextIndex < NumKinetic)
			{
				EulerFlowComposition<NumPotential, NumKinetic, PotentialIndex, NextIndex, Rest...>(PotentialWeights,
					KineticWeights,
					PotentialTickRateWeights,
					KineticTickRateWeights,
					Mass,
					Pos,
					LinMom,
					Dt);
			}
		}
	}

	//template<int N, int M>
	//void EulerFlowCompositionOld(const float A[N], 
	//	const float B[M],
	//	const float C[N], 
	//	const float D[M],
	//	const EulerIntegrationCompositionMode Modes[N + M],
	//	const float Mass,
	//	Eigen::Vector3f& Pos,
	//	Eigen::Vector3f& LinMom,
	//	float Dt = Globals::PhysicsTickLength)
	//{
	//	const int NumSteps = N + M;
	//	for (int Index = 0; Index < NumSteps; ++Index)
	//	{
	//		switch (Modes[Index]) 
	//		{
	//			case EulerIntegrationCompositionMode::Kinetic:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % N;
	//				const float PotentialDt = C[ModIndex] * Dt;
	//				EulerPotentialOnly(Mass,
	//					Pos, 
	//					LinMom, 
	//					A[ModIndex],
	//					PotentialDt);
	//				break;
	//			}
	//			case EulerIntegrationCompositionMode::Potential:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % M;
	//				const float KineticDt = D[ModIndex] * Dt;
	//				EulerKineticOnly(Mass,
	//					Pos, 
	//					LinMom,
	//					B[ModIndex],
	//					KineticDt);
	//				break;
	//			}
	//			default: 
	//			{
	//				//LOG this
	//			}
	//		}

	//	}
	//}
}