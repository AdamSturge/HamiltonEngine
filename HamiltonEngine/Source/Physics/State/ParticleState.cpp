#include "PrecompiledHeader/Pch.h"

#include "ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ParticleStateComponent::ParticleStateComponent(float M,
		const Eigen::Vector3f& Pos, 
		const Eigen::Vector3f& LinMom)
		: Mass{ M }
		,Position {Pos}
		, LinearMomentum{ LinMom }
	{
	}
}