#pragma once

#include "ParticleState.h"

namespace Eigen 
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	struct OrientationComponent
	{
		OrientationComponent(const Eigen::Matrix3f& O);
		Eigen::Matrix3f Orientation;
	};
	
	struct AngularMomentumComponent
	{
		AngularMomentumComponent(const Eigen::Vector3f& AngMom);
		Eigen::Vector3f AngularMomentum;
	};

	
	struct InertiaTensorComponent
	{
		InertiaTensorComponent() = default;
		InertiaTensorComponent(const Eigen::Diagonal3f& I);
		Eigen::Diagonal3f InertiaTensor;
	};
}