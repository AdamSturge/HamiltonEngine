#pragma once
#include "ConfigurationVariable.h"

namespace HamiltonEngine 
{
	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<std::string>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<float>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<double>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<int>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<unsigned int>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<long long>& Val)
	{
		Os << Val.Get();
		return Os;
	}

	std::ostream& operator<<(std::ostream& Os, const ConfigurationVariable<unsigned long long>& Val)
	{
		Os << Val.Get();
		return Os;
	}
}