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

    void save(cereal::JSONInputArchive& Record, const RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(Component.Mass);
    }

    void load(cereal::JSONInputArchive& Record, RigidBodyStateComponent& Component, const std::uint32_t Version)
    {
        Record(Component.Mass);
        //Record(m.x, m.y, m.z);
    }
}