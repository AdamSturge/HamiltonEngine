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
		bool ConfigVarOrDefault(const char* Key, T& Val, const T& DefaultValue) const
		{
			auto Iter = ConfigJson.find(Key);
			if (Iter != ConfigJson.end())
			{
				Val = *Iter;
				return true;
			}

			Val = DefaultValue;
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
		void InitializeImpl(const char* filename);

		bool Initialized = false;
		nlohmann::json ConfigJson;
		std::unordered_map<std::string, ConfigurationVariableBase*> PreInitializationVars;
	};
}