#pragma once
#include "ConfigurationSystem.h"
#include "Configuration/ConfigurationVariableBase.h"
#include <ostream>

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

		const T& Get() const 
		{
			return Storage;
		}

		operator T() const { return Storage; }
		operator T&() { return Storage; }
		operator const T&() const { return Storage; }
		friend std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<T>& Val);
	private:
		T Storage;
	};

	template<class T>
	std::ostream& operator<<(std::ostream& Os, const HamiltonEngine::ConfigurationVariable<T>& Val)
	{
		Os << Val.Get();
		return Os;
	}
}
