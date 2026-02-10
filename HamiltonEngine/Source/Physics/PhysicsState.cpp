#include "PrecompiledHeader/Pch.h"

#include "PhysicsState.h"
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

    OrientationComponent::OrientationComponent(const Eigen::Vector3f& Or) : Orientation{ Or }
    {

    }

    AngularMomentumComponent::AngularMomentumComponent(const Eigen::Vector3f& Ang) : AngularMomentum{ Ang }
    {
    }

    InertiaTensorComponent::InertiaTensorComponent(const Eigen::Matrix3f& I) : Inertia{ I }
    {
    }
}