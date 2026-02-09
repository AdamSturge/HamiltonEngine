#include "PrecompiledHeader/Pch.h"

#include "ConfigurationVariable.h"

namespace HamiltonEngine 
{
	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<std::string>& Val)
	{
		Os << Val.Get();
		return Os;
	}
}