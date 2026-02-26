#include "PrecompiledHeader/Pch.h"

#include "EulerB.h"
#include "EulerFlowComposition.h"

namespace HamiltonEngine::Physics
{
	void EulerB(const float Mass,
		Eigen::Vector3f& Position,
		Eigen::Vector3f& LinearMomentum,
		entt::const_handle PotentialEnergyEntity,
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
			EulerIntegrationCompositionMode::Potential,
			EulerIntegrationCompositionMode::Kinetic>(PotentialWeights,
				KineticWeights,
				PotentialTickRateWeights,
				KineticTickRateWeights,
				Mass,
				Position,
				LinearMomentum,
				PotentialEnergyEntity,
				Dt);
	}
}
