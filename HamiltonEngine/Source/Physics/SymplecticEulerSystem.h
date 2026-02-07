#pragma once
#include "PhysicsState.h"

namespace HamiltonEngine::Physics
{
	void SymplecticEulerSystem(PositionComponent& Pos, LinearMomentumComponent& LinMom);
}