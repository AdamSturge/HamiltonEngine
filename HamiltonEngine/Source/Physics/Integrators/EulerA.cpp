#include "PrecompiledHeader/Pch.h"

#include "EulerA.h"
#include "HamiltonianSplitting.h"

namespace HamiltonEngine::Physics
{
	void EulerA(const float Mass,
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
		constexpr EulerMode Modes[N + M]{ EulerMode::KineticOnly, EulerMode::PotentialOnly };

		HamiltonianSplitting<N, M>(A, B, C, D, Modes, Mass, Pos, LinMom, Dt);
	}
}
