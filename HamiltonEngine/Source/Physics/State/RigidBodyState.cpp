#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    RigidBodyStateComponent::RigidBodyStateComponent(
        const Eigen::Diagonal3f& I, 
        const Eigen::Affine3f& Trans, 
        const Eigen::Vector3f& AngMom)
        : InertiaTensor{I}
        , Transform{Trans}
        , AngularMomentum{AngMom}
    {
    }
}