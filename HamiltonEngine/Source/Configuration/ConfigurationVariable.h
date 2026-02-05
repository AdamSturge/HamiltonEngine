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
			Storage = DefaultValue;
			
			const ConfigurationSystem& Config = ConfigurationSystem::Get();
			if (Config.IsInitialized()) 
			{
				Config.LoadVarFromConfig(Key, Storage);
			}
			else 
			{
				ConfigurationSystem::GetMutable().RegisterForUpdateOnInitialization(Key, *this);
			}
		}

		virtual void SetValue(const AnyJsonVal& Val) override
		{
			Storage = Val.template get<T>();
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
}
