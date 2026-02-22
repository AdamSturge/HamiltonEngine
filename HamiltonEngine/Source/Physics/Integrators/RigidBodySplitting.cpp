#include "PrecompiledHeader/Pch.h"

#include "RigidBodySplitting.h"
#include "Physics/Potentials/PotentialEnergy.h"

namespace  HamiltonEngine::Physics
{	
	void RigidBodyPotentialOnly(
		float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		const RigidBodyPotentialEnergyComponent& PotentialEnergyComponent,
		float PotentialWeight, 
		float Dt)
	{
		Eigen::Vector3f PotentialEnergyLinearGrad;
		Eigen::Vector3f PotentialEnergyAngularGrad;
		ComputeGradPotentialEnergy(Transform, PotentialEnergyComponent, PotentialEnergyLinearGrad, PotentialEnergyAngularGrad);
	
		AngularMomentum -= PotentialEnergyAngularGrad * Dt;
		
		//TODO Linear update
		LinearMomentum -= Dt * PotentialEnergyLinearGrad;
	}

	/*
		We breakup H = T as H = TX + TY + TZ one kinetic term per moment of inertia
		T_i = (pi_i)^2/(2*l_i) where pi_i is the ith angular momentum in body coordinates and
		l_i is the ith moment of inertia in body coodinates. ie.) I = diag(lx,ly,lz)
		Using H = T_i we can solve for the analytic solution:
		- pi(t)  = exp(tA_i)pi(0) for the body angular momentum vector pi
		- s_j(t) = exp(tA_i)s_j(0) for columns s_j of the orientation matrix, j=0,1,2
		The A_i matrices have a particular form so that exponential matrices are directly
		coded below
	*/
	void RigidBodyKineticXOnly(
		float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight,
		float Dt)
	{
		Eigen::Vector3f MomentsOfIntertia = InertiaTensor.diagonal();
		const float w0 = AngularMomentum(0) / MomentsOfIntertia(0);

		const Eigen::Matrix3f EX{ 
			{1.0f, 0.0f,          0.0f},
			{0.0f, cos(w0 * Dt),  sin(w0 * Dt)},
			{0.0f, -sin(w0 * Dt), cos(w0 * Dt)}
		};

		AngularMomentum = EX * AngularMomentum;
		Transform = EX * Transform;

		Eigen::Vector3f DeltaLinearPosition(Dt * (1.0f / Mass) * LinearMomentum(0), 0.0f, 0.0f);
		Transform.translate(DeltaLinearPosition);
	}

	void RigidBodyKineticYOnly(
		float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight,
		float Dt)
	{
		Eigen::Vector3f MomentsOfIntertia = InertiaTensor.diagonal();
		const float w1 = AngularMomentum(1) / MomentsOfIntertia(1);

		const Eigen::Matrix3f EY{
			{cos(w1 * Dt),  0.0f, sin(w1 * Dt)},
			{0.0f,          1.0f, 0.0f},
			{-sin(w1 * Dt), 0.0f, cos(w1 * Dt)}
		};

		AngularMomentum = EY * AngularMomentum;
		Transform = EY * Transform;

		Eigen::Vector3f DeltaLinearPosition(0.0f, Dt * (1.0f / Mass) * LinearMomentum(1), 0.0f);
		Transform.translate(DeltaLinearPosition);
	}

	void RigidBodyKineticZOnly(float Mass,
		Eigen::Vector3f& LinearMomentum,
		Eigen::Diagonal3f& InertiaTensor,
		Eigen::Affine3f& Transform,
		Eigen::Vector3f& AngularMomentum,
		float KineticWeight,
		float Dt)
	{
		Eigen::Vector3f MomentsOfIntertia = InertiaTensor.diagonal();
		const float w2 = AngularMomentum(2) / MomentsOfIntertia(2);

		const Eigen::Matrix3f EZ{
			{cos(w2 * Dt),  -sin(w2 * Dt), 0.0f},
			{sin(w2 * Dt),  cos(w2 * Dt),  0.0f},
			{0.0f,          0.0f,          1.0f}
		};

		AngularMomentum = EZ * AngularMomentum;
		Transform = EZ * Transform;

		Eigen::Vector3f DeltaLinearPosition(0.0f, 0.0f, Dt * (1.0f / Mass) * LinearMomentum(2));
		Transform.translate(DeltaLinearPosition);
	}
}
