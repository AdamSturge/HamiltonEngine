#pragma once
#include "Logging/Logging.h"

namespace HamiltonEngine::Serialization 
{
	template<typename Type>
	void EnttSave(cereal::JSONOutputArchive& Archive, entt::registry const& Registry)
	{
		entt::snapshot{ Registry }.template get<Type>(Archive);
	}

	template<typename Type>
	void EnttLoad(cereal::JSONInputArchive& Archive, entt::registry& Registry)
	{
		entt::snapshot_loader{ Registry }.template get<Type>(Archive);
	}

	// Base template for component registration
	template<typename T>
	void RegisterComponentForSerialization(const char* Name)
	{
		using namespace entt::literals; // For "_hs" literal

		//TODO Unclear if these need to stick around
		entt::meta_factory<T>()
			//.type("name"_hs)
			.type(Name)
			.template func<&EnttSave<T>>("EnttSave"_hs)
			.template func<&EnttLoad<T>>("EnttLoad"_hs);
	};

	void Save(cereal::JSONInputArchive& Record, entt::registry const& Registry);

	void Load(cereal::JSONInputArchive& Record, entt::registry& Registry);

	//Call to register all components for save/load
	void RegisterComponents();
}
