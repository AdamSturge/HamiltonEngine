#include "Configuration/ConfigurationVariable.h"
#include "PhysicsState.h"

namespace HamiltonEngine::Physics
{
    HamiltonEngine::Physics::PositionComponent::PositionComponent(Eigen::Vector3f Pos) : Position{Pos}
    {
        
    }

    LinearMomentumComponent::LinearMomentumComponent(Eigen::Vector3f Mom) : LinearMomentum{Mom}
    {
    }

    MassComponent::MassComponent(float M) : Mass{M}
    {
    }
}