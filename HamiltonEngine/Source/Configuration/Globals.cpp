#include <Configuration/ConfigurationVariable.h>

namespace HamiltonEngine::Globals
{
	ConfigurationVariable<float> PhysicsTickLength("PhysicsTickLength", 1.0f / 60.0f);
	ConfigurationVariable<std::vector<float>> BackgroundColorRGB("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
	ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
	ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
}