#pragma once
#include "ConfigurationSystem.h"
#include "Configuration/ConfigurationVariableBase.h"

namespace HamiltonEngine 
{

	template<class T>
	class ConfigurationVariable : public ConfigurationVariableBase
	{
	public:
		ConfigurationVariable(const char* Key, T DefaultValue = T())
		{
			const ConfigurationSystem& Config = ConfigurationSystem::Get();
			if (Config.IsInitialized()) 
			{
				Config.ConfigVarOrDefault(Key, Storage, DefaultValue);
			}
			else 
			{
				ConfigurationSystem::GetMutable().RegisterForUpdateOnInitialization(Key, *this);
			}
		}

		virtual void SetValue(const AnyJsonVal& Val) override
		{
			Storage = Val;
		}

		operator T() const { return Storage; }
		operator T&() { return Storage; }
		operator const T&() const { return Storage; }
	private:
		T Storage;
	};
}