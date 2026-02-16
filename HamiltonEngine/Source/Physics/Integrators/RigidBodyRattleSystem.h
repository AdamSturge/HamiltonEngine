#pragma once

namespace HamiltonEngine::Physics
{
	struct TransformComponent;
	struct AngularMomentumComponent;
	struct MassTensorComponent;
	struct GradRigidBodyPotentialComponent;

	void RigidBodyRattleSystem(TransformComponent& TransC,
		AngularMomentumComponent& AngularMomC,
		const MassTensorComponent MassTensorC,
		const GradRigidBodyPotentialComponent& GradPotential);
}