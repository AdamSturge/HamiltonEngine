#include "PrecompiledHeader/Pch.h"

#include "ConfigurationSystem.h"

namespace HamiltonEngine 
{
	ConfigurationSystem Singleton<ConfigurationSystem>::Storage;

	bool ConfigurationSystem::RegisterForUpdateOnInitialization(const char* Key, ConfigurationVariableBase& Var)
	{
		if (PreInitializationVars.contains(Key))
		{
			HAMILTON_LOG(Configuration, 
				Warning,
				"Attempting multiple initialization of config variable with key: %s",
				Key)
			return false; 
		}

		PreInitializationVars[Key] = &Var;
		return true;
	}

	bool ConfigurationSystem::IsInitialized() const
	{
		return Initialized;
	}

	void ConfigurationSystem::InitializeImpl(const char* GlobalConfig, const char* UserConfig)
	{
		const bool GlobalConfigSuccess = ReadFile(GlobalConfig, GlobalConfigJson);
		const bool UserConfigSuccess = ReadFile(UserConfig, UserConfigJson); //user config is optional

		//We are done updating the config vars that registered for updates
		PreInitializationVars.clear();

		Initialized = GlobalConfigSuccess;
	}

	bool ConfigurationSystem::ReadFile(const char* Filename, nlohmann::json& Json) const
	{
		std::ifstream Filestream;
		try 
		{
			if (!std::filesystem::exists(Filename))
			{
				HAMILTON_LOG(Configuration,
					Warning,
					"Attempting to open file %s but file does not exist",
					Filename)
				return false;
			}

			Filestream.open(Filename);
		}
		catch (std::filesystem::filesystem_error e) 
		{
			HAMILTON_LOG(Configuration,
				Warning,
				"Attempting to open file %s but failed with exception: %s",
				Filename,
				e.what())
			return false;
		}

		Json = nlohmann::json::parse(Filestream);

		// Initialize the variables that tried to initialize before the config system was initialized
		for (auto& [Key, Value] : Json.items())
		{
			auto Iter = PreInitializationVars.find(Key.c_str());
			if (Iter != PreInitializationVars.end())
			{
				//ConfigurationVariableBase* Var = Iter->second;
				if (ConfigurationVariableBase* Var = Iter->second)
				{
					Var->SetValue(Value);
				}
				else
				{
					std::cout << "Hit a nullptr? wtf happened?" << std::endl;
				}
			}
		}

		return true;
	}
}