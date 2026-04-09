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

    void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(cereal::make_nvp("Translation", Component.Transform.translation()));
        Record(cereal::make_nvp("Mass",Component.Mass));
    }

    void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Physics::RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(Component.Mass);
    }
}
