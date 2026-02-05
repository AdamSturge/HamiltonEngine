#pragma once
#include <Configuration/ConfigurationVariable.h>


namespace HamiltonEngine::Globals
{
	ConfigurationVariable<float> PhysicsTickLength("PhysicsTickLength", 1.0f / 60.0f);
}