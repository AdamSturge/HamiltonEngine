#include "PrecompiledHeader/Pch.h"

#include "ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
    PositionComponent::PositionComponent(const Eigen::Vector3f& Pos) : Position{Pos}
    {
    }
    
    LinearMomentumComponent::LinearMomentumComponent(const Eigen::Vector3f& Mom) : LinearMomentum{Mom}
    {
    }

    MassComponent::MassComponent(float M) : Mass{M}
    {
    }
}