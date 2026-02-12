#pragma once

namespace HamiltonEngine::Physics
{
	struct TransformComponent
	{
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

	MassTensorComponent InertiaTensorToMassTensor(const InertiaTensorComponent&);
	InertiaTensorComponent MassTensorToInertiaTensor(const MassTensorComponent&);
}