#pragma once
#include "PhysicsComponent.h"

namespace HamiltonEngine::Physics
{
	void SymplecticEulerSystem(PositionComponent& Pos, LinearMomentumComponent& LinMom);
}