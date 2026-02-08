#pragma once

#include "Config.h"
#include <Singleton/Singleton.h>
#include <Configuration/ConfigurationVariableBase.h>

#include <unordered_map>

#include <nlohmann/json.hpp>

namespace HamiltonEngine
{
	// Singleton to wrap the configuration file
	class ConfigurationSystem : public Singleton<ConfigurationSystem>
	{
		friend Singleton<ConfigurationSystem>;
	public:
		template<class T>
		bool LoadVarFromConfig(const char* Key, T& Val) const
		{
			auto UserIter = UserConfigJson.find(Key);
			if (UserIter != UserConfigJson.end())
			{
				Val = UserIter->template get<T>();

				return true;
			}
			
			auto GlobalIter = GlobalConfigJson.find(Key);
			if (GlobalIter != GlobalConfigJson.end())
			{
				Val = GlobalIter->template get<T>();
	
				return true;
			}

			return false;
		}

		bool RegisterForUpdateOnInitialization(const char* Key, ConfigurationVariableBase& Var);

		bool IsInitialized() const;
	private:
		//ctor is private for use in singleton parent class
		ConfigurationSystem() {};
		// delete copy construction and copy assignment
		ConfigurationSystem(ConfigurationSystem&) = delete;
		void operator=(ConfigurationSystem&) = delete;
		// Required by Singleton interface
		void InitializeImpl(const char* GlobalConfig, const char* UserConfig);

		bool ReadFile(const char* Filename, nlohmann::json& Json) const;

		bool Initialized = false;
		nlohmann::json GlobalConfigJson;
		nlohmann::json UserConfigJson;
		std::unordered_map<std::string, ConfigurationVariableBase*> PreInitializationVars;
	};
}