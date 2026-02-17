#include "PrecompiledHeader/Pch.h"

#include "RigidBodySplitting.h"

namespace  HamiltonEngine::Physics
{
	void RigidBodyPotentialOnly(float Mass, 
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom, 
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom, 
		float PotentialWeight, 
		float Dt)
	{
	}

	void RigidBodyKineticOnly(float Mass,
		Eigen::Vector3f& Pos,
		Eigen::Vector3f& LinMom,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom,
		float KineticWeight,
		float Dt)
	{
	}
}
