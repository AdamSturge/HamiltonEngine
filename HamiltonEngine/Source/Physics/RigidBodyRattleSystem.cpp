#include "PrecompiledHeader/Pch.h"

#include "RigidBodyRattleSystem.h"
#include "ParticleState.h"
#include "Physics/RigidBodyState.h"
#include "Configuration/Globals.h"

namespace 
{
	HamiltonEngine::ConfigurationVariable<int> RattleNewtonIterMax("RigidBodyRattleNewtonIterMax", 5);
	
	//TODO debug this
	void QuasiNewtonRotation(
		Eigen::Matrix3f& Q, //InOut Param
		Eigen::Matrix3f& QBar, 
		const Eigen::Matrix3f& RInv,
		const Eigen::Matrix3f& LambdaBarR,
		const Eigen::Matrix3f& FirstGuess)
	{
		//Not good enough so now do quasi-newton type iteration to find Q
		Eigen::Matrix3f QPrev = Q;
		Eigen::Matrix3f Qk = FirstGuess;
		Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
		for (int K = 0; K < RattleNewtonIterMax; ++K)
		{
			// Update Lagrange multiplers based on new guess
			QBar = QBar - QPrev + Qk;
			Eigen::Matrix3f M = (QBar.transpose() * QBar) - I;
			Eigen::Matrix3f LambdaBar = LambdaBarR.array() * M.array();

			// It's kind of odd that we use the pre-update Qk in the stopping condition.
			Eigen::Matrix3f StoppingMatrix = (Qk.transpose() * Qk - I) - (LambdaBar * RInv - RInv * LambdaBar);
			const bool AllNearZero = StoppingMatrix.cwiseLessOrEqual(HamiltonEngine::Globals::Epsilon).all();

			QPrev = Qk;
			Qk = Qk - Q * LambdaBar * RInv;
			
			if (AllNearZero)
			{
				break;
			}
		}

		// Update output param
		Q = Qk;
	}
}

namespace HamiltonEngine::Physics
{
	//TODO this will need an optimization pass
	void RigidBodyRattleSystem(TransformComponent& TransC,
		AngularMomentumComponent& AngularMomC,
		const MassTensorComponent MassTensorC,
		const GradRigidBodyPotentialComponent& GradPotenntialC)
	{ 
		//Using Q for variable name since the rotation is our generalized coordinate
		Eigen::Matrix3f Q = TransC.Transform.rotation();
		
		//R is the mass tensor, not the rotation!
		const Eigen::Matrix3f& R = MassTensorC.MassTensor;
		Eigen::Matrix3f RInv = R.inverse();

		const Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
		
		//Compute some matricies to put problem into a form where we can compute
		//Lagrange multipliers directly
		Eigen::Matrix3f QBar = Q 
			+ Globals::PhysicsTickLength * AngularMomC.AngularMomentum * RInv
			- 0.5f * Globals::PhysicsTickLength * Globals::PhysicsTickLength * GradPotenntialC.GradPotential * RInv;
		Eigen::Matrix3f M = (QBar.transpose() * QBar) - I;

		// Compute Legrange multipliers directly
		Eigen::Matrix3f LambdaBarR;
		for (int ColumnIndex = 0; ColumnIndex < 3; ++ColumnIndex) 
		{
			for (int RowIndex = 0; RowIndex < 3; ++RowIndex)
			{
				const float RII = R(RowIndex, RowIndex);
				const float RJJ = R(ColumnIndex, ColumnIndex);
				const float Denom = RII + RJJ;
				
				if (abs(Denom) > Globals::Epsilon) [[likely]]
				{
					LambdaBarR(RowIndex, ColumnIndex) = RII * RJJ / Denom;
				}
			}
		}

		//Elementwise product
		Eigen::Matrix3f LambdaBar = LambdaBarR.array() * M.array();

		// First approximation
		Eigen::Matrix3f Q0 = QBar - Q * LambdaBar * RInv;
		Eigen::Matrix3f StoppingCondition =	(Q0.transpose() * Q0 - I) - (LambdaBar * RInv - RInv * LambdaBar);
		
		//Check if first approximation is good enough
		bool AllNearZero = StoppingCondition.cwiseLessOrEqual(Globals::Epsilon).all();
		if (!AllNearZero)
		{
			QuasiNewtonRotation(Q, QBar, RInv, LambdaBarR, Q0);
		}
		else 
		{
			Q = Q0;
		}

		std::cout << Q.transpose() * Q << std::endl << std::endl;
	}
}