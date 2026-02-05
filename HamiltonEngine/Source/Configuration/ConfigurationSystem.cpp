#include "ConfigurationSystem.h"

#include <fstream>

namespace HamiltonEngine 
{
	ConfigurationSystem Singleton<ConfigurationSystem>::Storage;

	bool ConfigurationSystem::RegisterForUpdateOnInitialization(const char* Key, ConfigurationVariableBase& Var)
	{
		if (PreInitializationVars.contains(Key))
		{
			return false; //TODO logging
		}

		PreInitializationVars[Key] = &Var;
		return true;
	}

	bool ConfigurationSystem::IsInitialized() const
	{
		return Initialized;
	}

	void ConfigurationSystem::InitializeImpl(const char* filename)
	{
		std::ifstream f(filename);
		
		ConfigJson = nlohmann::json::parse(f);

		// Initialize the variables that tried to initialize before the config system was initialized
		for (auto& [Key, Value] : ConfigJson.items())
		{
			auto Iter = PreInitializationVars.find(Key.c_str());
			if (Iter != PreInitializationVars.end())
			{
				ConfigurationVariableBase* Var = Iter->second;
				Var->SetValue(Value);
			}
		}
		PreInitializationVars.clear();

		Initialized = true;
	}
}