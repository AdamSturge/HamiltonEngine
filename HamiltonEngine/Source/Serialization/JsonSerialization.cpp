#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Serialization
{
	void Save(cereal::JSONInputArchive& Record, entt::registry const& Registry)
	{
		using namespace entt::literals; // For "_hs" literal

		for (auto&& Storage : Registry.storage())
		{
			auto Type = entt::resolve(Storage.first);
			Type.invoke("EnttSave"_hs, {}, entt::forward_as_meta(Record), entt::forward_as_meta(Registry));
		}
	}

	void Load(cereal::JSONInputArchive& Record, entt::registry& Registry)
	{
		using namespace entt::literals; // For "_hs" literal

		for (auto&& Storage : Registry.storage())
		{
			auto Type = entt::resolve(Storage.first);
			Type.invoke("EnttLoad"_hs, {}, entt::forward_as_meta(Record), entt::forward_as_meta(Registry));
		}
	}
	
	void RegisterComponents()
	{
		//Place components here
		RegisterComponentForSerialization<Physics::RigidBodyStateComponent>("RigidBodyStateComponent");
	}
}
