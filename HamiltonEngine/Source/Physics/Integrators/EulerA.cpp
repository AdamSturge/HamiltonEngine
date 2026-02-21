#include "PrecompiledHeader/Pch.h"

#include "EulerA.h"
#include "EulerFlowComposition.h"

namespace HamiltonEngine::Physics
{
	void EulerA(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt)
	{
		constexpr int NumPotential = 1;
		constexpr int NumKinetic = 1;
		constexpr float PotentialWeights[NumPotential]{ 1.0f };
		constexpr float KineticWeights[NumKinetic]{ 1.0f };
		constexpr float PotentialTickRateWeights[NumPotential]{ 1.0f };
		constexpr float KineticTickRateWeights[NumKinetic]{ 1.0f };
		constexpr int PotentialIndex = 0;
		constexpr int KineticIndex = 0;

		EulerFlowComposition<NumPotential, NumKinetic,
			PotentialIndex, KineticIndex,
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
