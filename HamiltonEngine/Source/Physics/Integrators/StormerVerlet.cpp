#include "PrecompiledHeader/Pch.h"

#include "StormerVerlet.h"
#include "EulerFlowComposition.h"

namespace HamiltonEngine::Physics
{
	void StormerVerlet(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt)
	{
		//Note: While formally we are doing H = 0.5V + T + 0.5V the below code is
		//using 1.0 weights for each V step and instead a 0.5 scale to the timestep.
		//This is mathematically equivalent. 
		//TODO experiment to see if using 0.5 weights for V makes a difference
		
		constexpr int NumPotential = 2;
		constexpr int NumKinetic = 1;
		constexpr float PotentialWeights[NumPotential]{ 1.0f };
		constexpr float KineticWeights[NumKinetic]{ 1.0f };
		constexpr float PotentialTickRateWeights[NumPotential]{ 0.5f,0.5f };
		constexpr float KineticTickRateWeights[NumKinetic]{ 1.0f };
		constexpr int PotentialIndex = 0;
		constexpr int KineticIndex = 0;

		EulerFlowComposition<NumPotential, NumKinetic, 
			PotentialIndex, KineticIndex,
			EulerIntegrationCompositionMode::Potential,
			EulerIntegrationCompositionMode::Kinetic, 
			EulerIntegrationCompositionMode::Potential>(PotentialWeights,
				KineticWeights,
				PotentialTickRateWeights,
				KineticTickRateWeights, 
				Mass,
				Pos,
				LinMom, 
				Dt);
	}
}
