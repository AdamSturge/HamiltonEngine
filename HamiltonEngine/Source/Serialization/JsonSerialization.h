#pragma once
#include "Logging/Logging.h"
#include "Version.h"

//TODO: We need these includes for the versioning below. Can we change that?
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Serialization 
{
	struct JsonEntityWrapper
	{
		SERIALIZATION_VERSION(JsonEntityWrapper,1)
		
		size_t Count = 0;
		std::vector<entt::entity> Values;
	};

	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::JsonEntityWrapper& Wrapper, const std::uint32_t Version);

	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::JsonEntityWrapper& Wrapper, const std::uint32_t Version);
	
	template<typename T>
	struct EntityComponentPair 
	{
		SERIALIZATION_VERSION(EntityComponentPair<T>,1)

		entt::entity Entity;
		T Component;
	};

	template<class T>
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::EntityComponentPair<T>& Pair, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("EntityId", Pair.Entity));
		Record(cereal::make_nvp("Component", Pair.Component));
	}

	template<class T>
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::EntityComponentPair<T>& Pair, const std::uint32_t Version)
	{
		Record(Pair.Entity);
		Record(Pair.Component);
	}
	
	template<class T>
	struct JsonComponentWrapper
	{
		SERIALIZATION_VERSION(JsonComponentWrapper<T>, 1)
		
		size_t Count = 0;
		std::vector<EntityComponentPair<T>> Values;
	};

	template<class T>
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::JsonComponentWrapper<T>& Wrapper, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("Count", Wrapper.Count));
		Record(cereal::make_nvp("Components", Wrapper.Values));
	}

	template<class T>
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::JsonComponentWrapper<T>& Wrapper, const std::uint32_t Version)
	{
		Record(Wrapper.Count);
		Record(Wrapper.Values);
	}
	
	bool SerializeEnttRegistryAsJson(const entt::registry& Registry, 
		const char* Filename, bool Overwrite);

	bool DeserializeEnttRegistryFromJson(entt::registry& Registry,
		const char* Filename);
}