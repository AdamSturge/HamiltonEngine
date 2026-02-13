#pragma once
#include <Configuration/ConfigurationVariable.h>


namespace HamiltonEngine::Globals
{
	extern ConfigurationVariable<float> PhysicsTickLength;
	extern ConfigurationVariable<bool> PhysicsSimEnabled;
	
	// small number. Useful for comparison to near 0
	extern ConfigurationVariable<float> Epsilon;
}