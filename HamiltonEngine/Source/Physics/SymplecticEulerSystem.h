#pragma once

#include "Physics/PhysicsState.h"

namespace HamiltonEngine::Physics
{
	void SymplecticEulerSystem(PositionComponent& Pos, LinearMomentumComponent& LinMom);
}