#pragma once
#include "Logging/Logging.h"
#include "Version.h"

//TODO: We need these includes for the versioning below. Can we change that?
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Serialization 
{
	bool SerializeEnttRegistryAsJson(const entt::registry& Registry, 
		const char* Filename, bool Overwrite);

	bool DeserializeEnttRegistryFromJson(entt::registry& Registry,
		const char* Filename);
}