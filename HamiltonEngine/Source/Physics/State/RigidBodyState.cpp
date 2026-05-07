#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    RigidBodyStateComponent::RigidBodyStateComponent()
    {
        Transform.setIdentity();
        LinearMomentum.setZero();
        AngularMomentum.setZero();
        InertiaTensor.setIdentity();
    }

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

    void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(cereal::make_nvp("Transform", Component.Transform));
        Record(cereal::make_nvp("Mass",Component.Mass));
        Record(cereal::make_nvp("LinearMomentum",Component.LinearMomentum));
        Record(cereal::make_nvp("InertiaTensor",Component.InertiaTensor));
        Record(cereal::make_nvp("AngularMomentum",Component.AngularMomentum));
    }

    void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(Component.Transform);
        Record(Component.Mass);
        Record(Component.LinearMomentum);
        Record(Component.InertiaTensor);
        Record(Component.AngularMomentum);
    }
}
