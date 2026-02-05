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

		//Annoying that non strings have to have this. You can't use enable_if on friend functions either
		friend std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<std::string>& Val);
	private:
		T Storage;
	};
}
