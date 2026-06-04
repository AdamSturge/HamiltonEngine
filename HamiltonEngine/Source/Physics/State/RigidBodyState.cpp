#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    RigidBodyStateComponent::RigidBodyStateComponent()
    {
        Transform.setIdentity();
        Mass = 1.0f;
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

    SERIALIZATION_DEFINITION_SAVE_COMPONENT_DEFAULT(HamiltonEngine::Physics::RigidBodyStateComponent)
    {
        Record(cereal::make_nvp("Transform", Component.Transform));
        Record(cereal::make_nvp("Mass",Component.Mass));
        Record(cereal::make_nvp("LinearMomentum",Component.LinearMomentum));
        Record(cereal::make_nvp("InertiaTensor",Component.InertiaTensor));
        Record(cereal::make_nvp("AngularMomentum",Component.AngularMomentum));
        Record(cereal::make_nvp("PotentialEnergyListHead",Component.PotentialEnergyListHead.entity()));
    }

    SERIALIZATION_DEFINITION_LOAD_COMPONENT_DEFAULT(HamiltonEngine::Physics::RigidBodyStateComponent)
    {
        Record(cereal::make_nvp("Transform",Component.Transform));
        Record(cereal::make_nvp("Mass",Component.Mass));
        Record(cereal::make_nvp("LinearMomentum",Component.LinearMomentum));
        Record(cereal::make_nvp("InertiaTensor",Component.InertiaTensor));
        Record(cereal::make_nvp("AngularMomentum",Component.AngularMomentum));

        entt::entity PotentialEnergyListHead{};
        Record(cereal::make_nvp("PotentialEnergyListHead", PotentialEnergyListHead));
        Component.PotentialEnergyListHead = entt::const_handle(Globals::Registry, PotentialEnergyListHead);
    }
}
