#pragma once

namespace HamiltonEngine::Physics
{
	float ComputeConstantGravityPotential(const Eigen::Vector3f& Pos,
		float Mass);

	void ComputeGradConstantGravityPotential(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy);
}