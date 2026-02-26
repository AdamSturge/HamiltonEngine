#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    RigidBodyStateComponent::RigidBodyStateComponent(
        const Eigen::Affine3f& Trans,
        float M,
        const Eigen::Vector3f& LinearMomentum,
        const Eigen::Diagonal3f& I,
        const Eigen::Vector3f& AngMom)
        : Transform{Trans}
        , Mass{M}
        , LinearMomentum{LinearMomentum}
        , InertiaTensor{I}
        , AngularMomentum{AngMom}
    {
    }

    RigidBodyPotentialEnergyListComponent::RigidBodyPotentialEnergyListComponent(entt::const_handle Parent) : RigidBodyEntity{Parent}
    {
    }

}