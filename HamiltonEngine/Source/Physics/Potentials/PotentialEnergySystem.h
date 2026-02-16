#pragma once

namespace HamiltonEngine::Physics 
{
	struct TransformComponent;
	struct MassComponent;
	using PotentialEnergyGradient = Eigen::Vector3f;
	
	float PotentialEnergySystem(const TransformComponent& TransformC,
		const MassComponent& MassC);

	//The assumption here is that the {V_i, V_j} = 0. ie.) The Poisson Bracket of the
	//potential energy functions being summed over is pairwaise 0. 
	//In such a case we can commute symplectic flow maps (so order doesn't matter)
	//and the flow map for the sum of the potential energies is the composition
	//of the flow maps for each individual potential energy
	//Phi_{V1 + V2 + ... VN, dt) = Phi_{VN,dt} o...o Phi_{V2,dt} o Phi_{V1,dt}(q)
	void GradPotentialEnergySystem(const TransformComponent& TransformC, 
		const MassComponent& MassC,
		PotentialEnergyGradient& OutGradPotentialEnergy);
}