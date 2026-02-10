#pragma once

namespace HamiltonEngine::Physics 
{
	struct PositionComponent 
	{
		PositionComponent(const Eigen::Vector3f& Pos);
		Eigen::Vector3f Position;
	};

	struct LinearMomentumComponent
	{
		LinearMomentumComponent(const Eigen::Vector3f& Mom);
		Eigen::Vector3f LinearMomentum;
	};

	struct MassComponent
	{
		MassComponent(float M);
		float Mass;
	};

	struct OrientationComponent
	{
		OrientationComponent(const Eigen::Vector3f& Or);
		Eigen::Vector3f Orientation;
	};

	struct AngularMomentumComponent
	{
		AngularMomentumComponent(const Eigen::Vector3f& Ang);
		Eigen::Vector3f AngularMomentum;
	};

	struct InertiaTensorComponent
	{
		InertiaTensorComponent(const Eigen::Matrix3f& I);
		Eigen::Matrix3f Inertia;
	};
}