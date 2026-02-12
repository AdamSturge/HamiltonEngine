#include "PrecompiledHeader/Pch.h"

#include "RigidBodyState.h"

namespace HamiltonEngine::Physics 
{
    TransformComponent::TransformComponent(const Eigen::Affine3f& Trans) : Transform{ Trans }
    {

    }

    AngularMomentumComponent::AngularMomentumComponent(const Eigen::Matrix3f& Ang) : AngularMomentum{ Ang }
    {
    }

    MassTensorComponent::MassTensorComponent(const Eigen::Matrix3f& M) : MassTensor{ M }
    {
    }

    InertiaTensorComponent::InertiaTensorComponent(const Eigen::Matrix3f& I) : InertiaTensor{I}
    {
    }

    MassTensorComponent InertiaTensorToMassTensor(const InertiaTensorComponent&) 
    {
        //TODO implement this
        return MassTensorComponent();
    }
    
    InertiaTensorComponent MassTensorToInertiaTensor(const MassTensorComponent&) 
    {
        //TODO implement this
        return InertiaTensorComponent();
    }
}