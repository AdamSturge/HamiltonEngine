#pragma once

#include "RigidBodySplitting.h"
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
		RigidBodyIntegrationCompositionMode First,
		RigidBodyIntegrationCompositionMode... Rest>
	inline void RigidBodyFlowComposition(const float* PotentialWeights,
		const float* KineticWeights,
		const float* PotentialTickRateWeights,
		const float* KineticTickRateWeights,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float Dt = Globals::PhysicsTickLength)
	{
		static_assert((1 + sizeof...(Rest)) == (NumPotential + NumKinetic - PotentialIndex - KineticIndex),
			"Length of composition mode list is incorrect. Please ensure your template arguments are correct");
		
		if constexpr (First == RigidBodyIntegrationCompositionMode::Potential &&
			PotentialIndex >= 0 && PotentialIndex < NumPotential)
		{
			const float PotentialDt = PotentialTickRateWeights[PotentialIndex] * Dt;
			RigidBodyPotentialOnly(
				InertiaTensor,
				Transform,
				AngularMomentum,
				PotentialWeights[PotentialIndex],
				PotentialDt);

			constexpr int NextIndex = PotentialIndex + 1;
			if constexpr (NextIndex < NumPotential || KineticIndex < NumKinetic)
			{
				RigidBodyFlowComposition<NumPotential, NumKinetic, 
					NextIndex, KineticIndex, 
					Rest...>(PotentialWeights,
					KineticWeights,
					PotentialTickRateWeights,
					KineticTickRateWeights,
					InertiaTensor,
					Transform,
					AngularMomentum,
					Dt);
			}
		}

		if constexpr (First == RigidBodyIntegrationCompositionMode::KineticX
			&& KineticIndex >= 0 && KineticIndex < NumKinetic)
		{
			const float KineticDt = KineticTickRateWeights[KineticIndex] * Dt;
			RigidBodyKineticXOnly(
				InertiaTensor,
				Transform,
				AngularMomentum,
				KineticWeights[KineticIndex],
				KineticDt);

			constexpr int NextIndex = KineticIndex + 1;
			if constexpr (PotentialIndex < NumPotential || NextIndex < NumKinetic)
			{
				RigidBodyFlowComposition<NumPotential, NumKinetic, 
					PotentialIndex, NextIndex, 
					Rest...>(PotentialWeights,
					KineticWeights,
					PotentialTickRateWeights,
					KineticTickRateWeights,
					InertiaTensor,
					Transform,
					AngularMomentum,
					Dt);
			}
		}

		if constexpr (First == RigidBodyIntegrationCompositionMode::KineticY
			&& KineticIndex >= 0 && KineticIndex < NumKinetic)
		{
			const float KineticDt = KineticTickRateWeights[KineticIndex] * Dt;
			RigidBodyKineticYOnly(
				InertiaTensor,
				Transform,
				AngularMomentum,
				KineticWeights[KineticIndex],
				KineticDt);

			constexpr int NextIndex = KineticIndex + 1;
			if constexpr (PotentialIndex < NumPotential || NextIndex < NumKinetic)
			{
				RigidBodyFlowComposition<NumPotential, NumKinetic,
					PotentialIndex, NextIndex,
					Rest...>(PotentialWeights,
						KineticWeights,
						PotentialTickRateWeights,
						KineticTickRateWeights,
						InertiaTensor,
						Transform,
						AngularMomentum,
						Dt);
			}
		}

		if constexpr (First == RigidBodyIntegrationCompositionMode::KineticZ
			&& KineticIndex >= 0 && KineticIndex < NumKinetic)
		{
			const float KineticDt = KineticTickRateWeights[KineticIndex] * Dt;
			RigidBodyKineticZOnly(
				InertiaTensor,
				Transform,
				AngularMomentum,
				KineticWeights[KineticIndex],
				KineticDt);

			constexpr int NextIndex = KineticIndex + 1;
			if constexpr (PotentialIndex < NumPotential || NextIndex < NumKinetic)
			{
				RigidBodyFlowComposition<NumPotential, NumKinetic,
					PotentialIndex, NextIndex,
					Rest...>(PotentialWeights,
						KineticWeights,
						PotentialTickRateWeights,
						KineticTickRateWeights,
						InertiaTensor,
						Transform,
						AngularMomentum,
						Dt);
			}
		}
	}
	
	
	//Split the Hamiltonian H = T + V to produce higher order methods
	//A are the weights for the potnential energy split. H = a0V + a1*V (w0 + w1 = 1)
	//B are the weights for the kinentic energy split  H = b0T + b1T (b0 + b1 = 1)
	//C are the weights for the potnential energy dT substepping Phi_{V,dt} = Phi_{V,c1*dt} o Phi_{V,c0*dt} (c0 + c1 = 1)
	//D are the weights for the kinetic energy dT substepping Phi_{V,dt} = Phi_{T,d1*dt} o Phi_{T,d0*dt} (d0 + d1 = 1)
	//template<int N, int M>
	//void RigidBodyFlowComposition(const float A[N],
	//	const float B[M],
	//	const float C[N],
	//	const float D[M],
	//	const RigidBodyIntegrationCompositionMode Modes[N + M],
	//	Eigen::Diagonal3f& InertiaTensor,
	//	Eigen::Matrix3f& Transform,
	//	Eigen::Vector3f& AngularMomentum,
	//	float Dt = Globals::PhysicsTickLength)
	//{
	//	const int NumSteps = N + M;
	//	for (int Index = 0; Index < NumSteps; ++Index)
	//	{
	//		switch (Modes[Index])
	//		{
	//			case RigidBodyIntegrationCompositionMode::Potential:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % N;
	//				const float PotentialDt = C[ModIndex] * Dt;
	//				RigidBodyPotentialOnly(
	//					InertiaTensor,
	//					Transform,
	//					AngularMomentum,
	//					A[ModIndex],
	//					PotentialDt);
	//				break;
	//			}
	//			case RigidBodyIntegrationCompositionMode::KineticX:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % M;
	//				const float KineticDt = D[ModIndex] * Dt;
	//				RigidBodyKineticXOnly(
	//					InertiaTensor,
	//					Transform,
	//					AngularMomentum,
	//					B[ModIndex],
	//					KineticDt);
	//				break;
	//			}
	//			case RigidBodyIntegrationCompositionMode::KineticY:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % M;
	//				const float KineticDt = D[ModIndex] * Dt;
	//				RigidBodyKineticYOnly(
	//					InertiaTensor,
	//					Transform,
	//					AngularMomentum,
	//					B[ModIndex],
	//					KineticDt);
	//				break;
	//			}
	//			case RigidBodyIntegrationCompositionMode::KineticZ:
	//			{
	//				//Should never wrap but do mod just to avoid crashing in
	//				//case of bad inputs
	//				const int ModIndex = Index % M;
	//				const float KineticDt = D[ModIndex] * Dt;
	//				RigidBodyKineticZOnly(
	//					InertiaTensor,
	//					Transform,
	//					AngularMomentum,
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