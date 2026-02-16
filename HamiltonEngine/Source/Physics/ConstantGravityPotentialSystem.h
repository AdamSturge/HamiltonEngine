#pragma once

namespace HamiltonEngine::Physics
{
	struct PositionComponent;
	struct MassComponent;
	using PotentialEnergyGradient = Eigen::Vector3f;

	float ConstantGravityPotentialSystem(const PositionComponent& PosC,
		const MassComponent& MassC);

	void GradConstantGravityPotentialSystem(const PositionComponent& PosC,
		const MassComponent& MassC,
		PotentialEnergyGradient& OutGradPotentialEnergy);
}