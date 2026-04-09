#pragma once
#include "Logging/Logging.h"

namespace HamiltonEngine::Serialization 
{
	bool SerializeEnttRegistryAsJson(const entt::registry& Registry, 
		const char* Filename, bool Overwrite);
}
