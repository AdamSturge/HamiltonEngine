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

	using PotentialEnergyFn = float (*)(const Eigen::Affine3f& BodyToWorldTransform, Eigen::Vector3f BodyPosition);
	using PotentialEnergyGradFn = void (*)(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition, 
		Eigen::Vector3f OutGradLinearPotentialEnergy,
		Eigen::Vector3f OutGradAngularPotentialEnergy);

	struct RigidBodyPotentialEnergyComponent
	{
		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given rigid body
		//TODO change these to EnTT handle
		entt::entity NextEntity{ entt::null };
		entt::entity RigidBodyEntity{ entt::null }; //back pointer to rigid body

		Eigen::Vector3f BodyPointOfApplication; //Body Coordinates
		PotentialEnergyFn ComputePotentialEnergyFn;
		PotentialEnergyGradFn ComputePotentialEnergyGradFn;
	};

}