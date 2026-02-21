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

	struct RigidBodyPotentialEnergyListHeadComponent 
	{
		//TODO change this to EnTT handle
		entt::entity Head{ entt::null };
	};

	using PotentialEnergyFn = float (*)(Eigen::Vector3f WorldPosition);
	using PotentialEnergyGradFn = void (*)(Eigen::Vector3f WorldPosition, Eigen::Vector3f OutGradPotentialEnergy);
	struct RigidBodyPotentialEnergyComponent
	{
		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given rigid body
		//TODO change these to EnTT handle
		entt::entity NextEntity{ entt::null };
		entt::entity RigidBodyEntity{ entt::null }; //back pointer to rigid body

		Eigen::Vector3f PointOfApplication;
		PotentialEnergyFn ComputePotentialEnergyFn;
		PotentialEnergyGradFn ComputePotentialEnergyGradFn;
	};

}