#include "PrecompiledHeader/Pch.h"

#include "ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ParticleStateComponent::ParticleStateComponent(float M,
		const Eigen::Vector3f& Position, 
		const Eigen::Vector3f& LinearMomentum)
		: Mass{ M }
		,Position {Position}
		, LinearMomentum{ LinearMomentum }
	{
	}

	ParticlePotentialEnergyListComponent::ParticlePotentialEnergyListComponent(entt::const_handle Parent) : ParticleEntity{ Parent }
	{
	}
}