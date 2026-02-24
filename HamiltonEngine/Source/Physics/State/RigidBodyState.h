#pragma once

#include "ParticleState.h"

namespace Eigen 
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	struct RigidBodyStateComponent 
	{
		RigidBodyStateComponent(const Eigen::Affine3f& Trans,
			float M,
			const Eigen::Vector3f& LinMom,
			const Eigen::Diagonal3f& I,
			const Eigen::Vector3f& AngMom);
		
		Eigen::Affine3f Transform; //Body To World
		
		//Linear state
		float Mass;
		Eigen::Vector3f LinearMomentum; //World coordinates
		
		//Angular State
		Eigen::Diagonal3f InertiaTensor; // Body Coordinates
		Eigen::Vector3f AngularMomentum; // Body Coordinates

		//Potential Energy
		entt::const_handle PotentialEnergyListHead;
	};
	
	using PotentialEnergyFn = float (*)(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor);
	using PotentialEnergyGradFn = void (*)(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor,
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy);

	struct RigidBodyPotentialEnergyComponent
	{
		RigidBodyPotentialEnergyComponent(entt::const_handle Parent,
			const Eigen::Vector3f& BodyPoC,
			PotentialEnergyFn PotentialFn,
			PotentialEnergyGradFn GradFn);
		
		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given rigid body
		entt::const_handle NextEntity;
		entt::const_handle RigidBodyEntity; //back pointer to rigid body

		Eigen::Vector3f BodyPointOfApplication; //Body Coordinates
		PotentialEnergyFn ComputePotentialEnergyFn;
		PotentialEnergyGradFn ComputePotentialEnergyGradFn;
	};

}