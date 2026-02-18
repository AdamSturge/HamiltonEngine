#include "PrecompiledHeader/Pch.h"

#include "RigidBodySplitting.h"

namespace  HamiltonEngine::Physics
{
	void RigidBodyPotentialOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom, 
		float PotentialWeight, 
		float Dt)
	{
	}

	void RigidBodyKineticOnly(
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Matrix3f& Orientation,
		Eigen::Vector3f& AngMom,
		float KineticWeight,
		float Dt)
	{
		//We breakup H = T as H = T1 + T2 + T3 one kinetic term per moment of inertia
		//T_i = (pi_i)^2/(2*l_i) where pi_i is the ith angular momentum in body coordinates and
		//l_i is the ith moment of inertia in body coodinates. ie.) I = diag(l1,l2,l3)
		//When we do this we can exactly solve each kinetic energy step.

		//TODO I maybe want to break this down further so that methods can set the order
		//of integration between T1, T2, and T3. This will be needed to create higher order
		//methods
		
		Eigen::Vector3f MomentsOfIntertia = InertiaTensor.diagonal();
		
		const float w0 = AngMom(0) / MomentsOfIntertia(0);
		const float w1 = AngMom(1) / MomentsOfIntertia(1);
		const float w2 = AngMom(2) / MomentsOfIntertia(2);

		const Eigen::Matrix3f E0{ 
			{1.0f, 0.0f,        0.0f},
			{0.0f, cos(w0*Dt),  sin(w0*Dt)},
			{0.0f, -sin(w0*Dt), cos(w0*Dt)}
		};

		const Eigen::Matrix3f E1{
			{cos(w1 * Dt),  0.0f, sin(w1 * Dt)},
			{0.0f,          1.0f, 0.0f},
			{-sin(w1 * Dt), 0.0f, cos(w1 * Dt)}
		};

		const Eigen::Matrix3f E2{
			{cos(w1 * Dt),  -sin(w1 * Dt), 0.0f},
			{sin(w1 * Dt),  cos(w1 * Dt),  0.0f},
			{0.0f,          0.0f,          1.0f}
		};

		const Eigen::Matrix3f E = E2 * E1 * E0;

		AngMom = E*AngMom;
		Orientation = E * Orientation;
	}
}
