#include "PrecompiledHeader/Pch.h"

#include "RigidBodyA.h"
#include "RigidBodyFlowComposition.h"

namespace HamiltonEngine::Physics
{
	void RigidBodyA(float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		entt::const_handle PotentialEnergyEntity)
	{
		constexpr int NumPotential = 1;
		constexpr int NumKinetic = 3;
		constexpr float PotentialWeights[NumPotential]{ 1.0f };
		constexpr float KineticWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
		constexpr float PotentialTickRateWeights[NumPotential]{ 1.0f };
		constexpr float KineticTickRateWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
		constexpr int PotentialIndex = 0;
		constexpr int KineticIndex = 0;

		RigidBodyFlowComposition<NumPotential, NumKinetic,
			PotentialIndex, KineticIndex,
			RigidBodyIntegrationCompositionMode::KineticX,
			RigidBodyIntegrationCompositionMode::KineticY,
			RigidBodyIntegrationCompositionMode::KineticZ,
			RigidBodyIntegrationCompositionMode::Potential>(PotentialWeights,
				KineticWeights,
				PotentialTickRateWeights,
				KineticTickRateWeights,
				Mass,
				LinearMomentum,
				InertiaTensor,
				Transform,
				AngularMomentum,
				PotentialEnergyEntity);
	}
}