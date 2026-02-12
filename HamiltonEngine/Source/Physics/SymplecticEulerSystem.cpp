#include "PrecompiledHeader/Pch.h"

#include "SymplecticEulerSystem.h"
#include "Physics/ParticleState.h"
#include "Configuration/Globals.h"

namespace HamiltonEngine::Physics 
{
	// Assumes H = T + U
	
	void SymplecticEulerSystem(PositionComponent& PosC,
		LinearMomentumComponent& LinMomC,
		const GradParticlePotentialComponent& GradUC)
	{
		const Eigen::Vector3f DeltaMomentum = -Globals::PhysicsTickLength * GradUC.GradPotential;
		LinMomC.LinearMomentum += DeltaMomentum;
		
		Eigen::Vector3f DeltaPosition = Globals::PhysicsTickLength * LinMomC.LinearMomentum;
		PosC.Position += DeltaPosition;
	}
}