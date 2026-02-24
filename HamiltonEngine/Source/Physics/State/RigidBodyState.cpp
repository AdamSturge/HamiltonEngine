#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    RigidBodyStateComponent::RigidBodyStateComponent(
        const Eigen::Affine3f& Trans,
        float M,
        const Eigen::Vector3f& LinMom,
        const Eigen::Diagonal3f& I,
        const Eigen::Vector3f& AngMom)
        : Transform{Trans}
        , Mass{M}
        , LinearMomentum{LinMom}
        , InertiaTensor{I}
        , AngularMomentum{AngMom}
    {
    }

    RigidBodyPotentialEnergyComponent::RigidBodyPotentialEnergyComponent(entt::const_handle Parent,
        const Eigen::Vector3f& BodyPoC,
        PotentialEnergyFn PotentialFn, 
        PotentialEnergyGradFn GradFn)
        : RigidBodyEntity{Parent}
        , BodyPointOfApplication{BodyPoC}
        , ComputePotentialEnergyFn{PotentialFn}
        , ComputePotentialEnergyGradFn{GradFn}
    {

    }
}