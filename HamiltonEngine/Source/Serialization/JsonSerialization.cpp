#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Serialization
{
	void RegisterComponents()
	{
		//Place components here
		RegisterComponentForSerialization<Physics::RigidBodyStateComponent>("RigidBodyStateComponent");
	}
}
