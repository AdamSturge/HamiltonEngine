#pragma once

namespace HamiltonEngine::Physics
{
	struct PositionComponent;
	struct LinearMomentumComponent;
	struct GradParticlePotentialComponent;

	void SymplecticEulerSystem(PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		const GradParticlePotentialComponent& GradUC);
}