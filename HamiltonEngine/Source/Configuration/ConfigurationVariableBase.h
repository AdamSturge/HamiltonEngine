#pragma once
#include <any>
#include <nlohmann/json.hpp>

namespace HamiltonEngine
{
	using AnyJsonVal = nlohmann::json_abi_v3_11_3::basic_json<std::map, std::vector, std::string, bool, __int64, unsigned __int64, double, std::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::vector<unsigned char, std::allocator<unsigned char>>, void>;
	
	class ConfigurationVariableBase
	{
	public:
		virtual void SetValue(const AnyJsonVal& Val) = 0;
	};
}