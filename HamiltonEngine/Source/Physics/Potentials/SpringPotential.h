#pragma once

#include "Physics/State/RigidBodyState.h"

namespace Eigen
{
	using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
}

namespace HamiltonEngine::Physics
{
	float ComputeSpringPotentialParticle(const Eigen::Vector3f& ParticlePosition,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength);

	void ComputeGradSpringPotentialParticle(const Eigen::Vector3f& Position,
		const Eigen::Vector3f& OtherEndOfSpringPosition,
		float SpringConstant,
		float RestLength,
		Eigen::Vector3f& OutGradPotentialEnergy);

	float ComputeSpringPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float SpringConstant,
		const Eigen::Diagonal3f& InertiaTensor);

	void ComputeGradSpringPotentialRigidBody(
		const Eigen::Affine3f& BodyToWorldTransform,
		Eigen::Vector3f BodyPosition,
		float SpringConstant,
		const Eigen::Diagonal3f& InertiaTensor,
		Eigen::Vector3f& OutGradLinearPotentialEnergy,
		Eigen::Vector3f& OutGradAngularPotentialEnergy);


	struct SpringPotentialComponent
	{
		SpringPotentialComponent(entt::const_handle Parent, float SpringConstant,
			float RestLength, const Eigen::Vector3f& BAnchorPoint);

		//This is a linked list that connects to entities that are designed to compute 
		//potential energies acting on a given entity
		entt::const_handle NextEntity;
		entt::const_handle ParentEntity; //Back pointer to parent entity

		float K; //Spring constant
		float L; //Rest length

		Eigen::Vector3f AnchorPointBody; //Body coordinates of point spring is anchored to on this entity
		
		entt::const_handle OtherEntity; //Entity on other end of the spring
	};
	
	//struct SpringComponent
	//{
	//	entt::const_handle Entity1; //Entity on one end of the spring
	//	entt::const_handle Entity2; //Entity on other end of the spring
	//	Eigen::Vector3f AttachPoint1; //Attach point for entity 1 in world coordinates
	//	Eigen::Vector3f AttachPoint2; //Attach point for entity 2 in world coordinates
	//	float K; //Spring constant
	//	float L; // Rest length
	//};

}