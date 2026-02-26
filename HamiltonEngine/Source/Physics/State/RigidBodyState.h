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
			const Eigen::Vector3f& LinearMomentum,
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

	struct RigidBodyPotentialEnergyListComponent 
	{
		RigidBodyPotentialEnergyListComponent(entt::const_handle Parent);
		
		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given rigid body
		entt::const_handle NextEntity;
		entt::const_handle RigidBodyEntity; //back pointer to rigid body
	};

}