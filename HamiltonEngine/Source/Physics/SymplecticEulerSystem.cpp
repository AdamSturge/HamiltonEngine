#include "PrecompiledHeader/Pch.h"

#include "SymplecticEulerSystem.h"
#include "Configuration/Globals.h"

namespace HamiltonEngine::Physics 
{
	// Assumes H = T + U
	
	Eigen::Vector3f GradU(const PositionComponent& PosC)
	{
		return Eigen::Vector3f::Zero();
	}

	void SymplecticEulerSystem(PositionComponent& PosC, LinearMomentumComponent& LinMomC)
	{
		const Eigen::Vector3f Force = GradU(PosC);

		//TODO override math operators on config vars for numeric types?
		LinMomC.LinearMomentum = LinMomC.LinearMomentum - Globals::PhysicsTickLength * Force;
		PosC.Position = PosC.Position + Globals::PhysicsTickLength * LinMomC.LinearMomentum;
	}
}