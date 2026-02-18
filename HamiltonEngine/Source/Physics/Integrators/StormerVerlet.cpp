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
		
		constexpr int N = 2;
		constexpr int M = 1;
		constexpr float A[N]{ 1.0f, 1.0f };
		constexpr float B[M]{ 1.0f };
		constexpr float C[N]{ 0.5f, 0.5f };
		constexpr float D[M]{ 1.0f };
		constexpr EulerIntegrationCompositionMode Modes[N + M]{
			EulerIntegrationCompositionMode::Potential, // half time step V
			EulerIntegrationCompositionMode::Kinetic, // full time step T
			EulerIntegrationCompositionMode::Potential }; //half time step V

		EulerFlowComposition<N, M>(A, B, C, D, Modes, Mass, Pos, LinMom, Dt);
	}
}
