#pragma once

/*
	EulerB is a version of symplectic Euler with the folowing update rule given a Hamiltonian
	H = T + V
	p_{n+1} = p_{n} - dt*GradV(q_n) <-- dp/dt = -dH/dq = -GradV(q)
	q_{n+1} = q_{n} + dt+p_{n+1}    <-- dq/dt = -dH/dp = dT/dp = p

	Writing H = H1 + H2 (H1 = T, H2 = V) we can split this update in two via a splitting/composition of flow method
	The first is H2 = V
	p_{n+1} = p_{n} - dt*GradV(q_n)
	q_{n+1} = q_{n}

	The second is H1 = T
	p_{n+1} = p_n
	q_{n+1} = q_{n} + dt+p_{n+1}
*/

namespace HamiltonEngine::Physics 
{
	//Change this to flags?
	enum class EulerBMode 
	{
		Kenetic,
		Potential,
		Both
	};

	struct MassComponent;
	struct PositionComponent;
	struct LinearMomentumComponent;

	void EulerModeBPotentialOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC);

	void EulerModeBKineticOnlySystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC);

	template<EulerBMode Mode>
	void EulerBSystem(const MassComponent& MassC,
		PositionComponent& PosC,
		LinearMomentumComponent& LinMomC)
	{
		if constexpr (Mode == EulerBMode::Kenetic)
		{
			EulerModeBKineticOnlySystem(MassC, PosC, LinMomC);
		}

		else if constexpr (Mode == EulerBMode::Potential)
		{
			EulerModeBPotentialOnlySystem(MassC, PosC, LinMomC);
		}

		if constexpr (Mode == EulerBMode::Both)
		{
			//Order is important here
			EulerModeBPotentialOnlySystem(MassC, PosC, LinMomC);
			EulerModeBKineticOnlySystem(MassC, PosC, LinMomC);
		}
	}
}