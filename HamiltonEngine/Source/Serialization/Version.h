#pragma once

namespace HamiltonEngine::Serialization
{
	template<typename Type, std::uint32_t Version>
	static std::uint32_t RegisterVersion()
	{
		::cereal::detail::StaticObject<::cereal::detail::Versions>::getInstance().mapping.emplace(std::type_index(typeid(Type)).hash_code(), Version);
		return Version;
	}
}

#define SERIALIZATION_VERSION(TYPE, VERSION) static inline const std::uint32_t Version = HamiltonEngine::Serialization::RegisterVersion<TYPE,VERSION>();