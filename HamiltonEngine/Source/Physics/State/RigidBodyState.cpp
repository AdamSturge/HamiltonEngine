#include "PrecompiledHeader/Pch.h"
#define ORIENTATION_CHECK

#include "RigidBodyState.h"

#ifdef ORIENTATION_CHECK
#include "Configuration/Globals.h"
#endif

namespace HamiltonEngine::Physics 
{

    OrientationComponent::OrientationComponent(const Eigen::Matrix3f& O) : Orientation {O}
    {
#ifdef ORIENTATION_CHECK
        const Eigen::Matrix3f StoppingCondition = (Orientation.transpose() * Orientation - Eigen::Matrix3f::Identity());
        const bool IsProperOrientation = StoppingCondition.cwiseLessOrEqual(Globals::Epsilon).all();
        //TODO Log this
        //TODO Assert this
#endif
    }

    AngularMomentumComponent::AngularMomentumComponent(const Eigen::Vector3f& AngMom) : AngularMomentum{ AngMom }
    {
    }


    InertiaTensorComponent::InertiaTensorComponent(const Eigen::Diagonal3f& I) : InertiaTensor{I}
    {
    }
}