#pragma once

#include "ParticleState.h"

namespace HamiltonEngine::Physics
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const Eigen::Affine3f& Trans);
		Eigen::Affine3f Transform;
	};
	
	struct AngularMomentumComponent
	{
		AngularMomentumComponent(const Eigen::Matrix3f& Ang);
		Eigen::Matrix3f AngularMomentum;
	};

	struct MassTensorComponent
	{
		MassTensorComponent() = default;
		MassTensorComponent(const Eigen::Matrix3f& M);
		Eigen::Matrix3f MassTensor;
	};

	struct InertiaTensorComponent
	{
		InertiaTensorComponent() = default;
		InertiaTensorComponent(const Eigen::Matrix3f& I);
		Eigen::Matrix3f InertiaTensor;
	};

	struct GradRigidBodyPotentialComponent
	{
		Eigen::Matrix3f GradPotential;
	};


	//TODO This stuff is bad. Components are on entities!
	MassTensorComponent InertiaTensorToMassTensor(const InertiaTensorComponent&);
	InertiaTensorComponent MassTensorToInertiaTensor(const MassTensorComponent&);

	PositionComponent TransformComponentToPositionComponent(const TransformComponent& TransformC);
	TransformComponent PositionComponentToTransformComponent(const PositionComponent& PositionC);
}