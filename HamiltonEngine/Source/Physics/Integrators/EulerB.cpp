#include "PrecompiledHeader/Pch.h"

#include "EulerB.h"
#include "EulerFlowComposition.h"

namespace HamiltonEngine::Physics
{
	void EulerB(const float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		float Dt)
	{
		constexpr int N = 1;
		constexpr int M = 1;
		constexpr float A[N]{ 1.0f };
		constexpr float B[M]{ 1.0f };
		constexpr float C[N]{ 1.0f };
		constexpr float D[M]{ 1.0f };
		constexpr CompositionMode Modes[N+M]{ CompositionMode::PotentialOnly, CompositionMode::KineticOnly };
		
		EulerFlowComposition<N,M>(A, B, C, D, Modes, Mass, Pos, LinMom, Dt);
	}
}
