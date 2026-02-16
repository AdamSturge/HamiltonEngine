#include "PrecompiledHeader/Pch.h"

#include "RigidBodyRattleSystem.h"
#include "ParticleState.h"
#include "Physics/RigidBodyState.h"
#include "Configuration/Globals.h"

namespace
{
	HamiltonEngine::ConfigurationVariable<int> RattleNewtonIterMax("RigidBodyRattleNewtonIterMax", 5);
	
	Eigen::Matrix3f Commutator(const Eigen::Matrix3f& A, const Eigen::Matrix3f& B)
	{
		return A * B - B * A;
	}
}

namespace HamiltonEngine::Physics
{
	void RattleAngularPosition(const Eigen::Matrix3f& QCurrent, //Q_{n}
		const Eigen::Matrix3f& PCurrent, // P_{n}
		const Eigen::Matrix3f& R, // Mass Matrix
		const Eigen::Matrix3f& GradPotenntialC, // Grad V(Q_{n})
		Eigen::Matrix3f QNext, // Output variable Q_{n+1}
		Eigen::Matrix3f QLagrangeMultiplers) // Output variable Lagrange multipliers
	{
		//R is the mass tensor, not the rotation!
		Eigen::Matrix3f RInv = R.inverse();
		const Eigen::Matrix3f I = Eigen::Matrix3f::Identity();

		// Compute mass matrix portion of Lagrange multipliers directly
		// Note the use of the Bar suffix here means we are actually computing
		// the mulitpler matrix scaled by DeltaT^2
		// The below mathematics accounts for this but we will want to rescale
		// before returning for use in the momentum update
		//TODO this matrix is symmetric, so maybe we can do half the work
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

		//Quasi-Newton scheme to meet Q^T*Q = I constraint
		bool Converged = false;
		Eigen::Matrix3f Qk = QCurrent;
		const Eigen::Matrix3f QBarConstant = Globals::PhysicsTickLength * PCurrent * RInv
			- 0.5f * Globals::PhysicsTickLength * Globals::PhysicsTickLength * GradPotenntialC * RInv;
		Eigen::Matrix3f QBar = Qk + QBarConstant;
		Eigen::Matrix3f QkPrev = QBar;
		int IterCount = 0;
		do
		{
			//Compute update to Qk
			Eigen::Matrix3f M = (QBar.transpose() * QBar) - I;
			QLagrangeMultiplers = LambdaBarR.array() * M.array();
			Qk = QkPrev - QCurrent * QLagrangeMultiplers * RInv;
			
			//Check for convergence
			Eigen::Matrix3f StoppingCondition = (Qk.transpose() * Qk - I)
				- Commutator(QLagrangeMultiplers, RInv);
			Converged = StoppingCondition.cwiseLessOrEqual(Globals::Epsilon).all();
			
			//Update for next iteration
			QBar = Qk + QBarConstant;
			QkPrev = Qk;			
			++IterCount;
		} while (!Converged && IterCount < RattleNewtonIterMax);

		//std::cout << "Converged in " << IterCount << " Iterations" << std::endl;
		
		//Update output parameters
		QNext = Qk;
		// Rescale to remove implicit DeltaT^2
		QLagrangeMultiplers = QLagrangeMultiplers.array() / (Globals::PhysicsTickLength * Globals::PhysicsTickLength);

		//std::cout << QNext.transpose() * QNext << std::endl << std::endl;
	}
	

	// Compute P_{n+1}
	void RattleAngularMomentum(
		const Eigen::Matrix3f& QCurrent, //Q_{n}
		const Eigen::Matrix3f& QNext, // Q_{n+1}
		const Eigen::Matrix3f& PCurrent, //P_{n}
		const Eigen::Matrix3f& MassTensor, //R
		const Eigen::Matrix3f& GradPotentialCurrent, //Grad V(Q_{n}
		const Eigen::Matrix3f& QLagrangeMultiplers,
		Eigen::Matrix3f& PNext) 
	{
		//TODO this remains to be implemetned
		
		//P_{n+1/2} is the momentum only considering the angular position constraint
		Eigen::Matrix3f PHHalf = PCurrent
			- 0.5f*Globals::PhysicsTickLength * GradPotentialCurrent
			- QCurrent * QLagrangeMultiplers;
	}
	
	//TODO this will need an optimization pass
	void RigidBodyRattleSystem(TransformComponent& TransC,
		AngularMomentumComponent& AngularMomC,
		const MassTensorComponent MassTensorC,
		const GradRigidBodyPotentialComponent& GradPotenntialC)
	{ 
		const Eigen::Matrix3f& QCurrent = TransC.Transform.rotation();
		const Eigen::Matrix3f& PCurrent = AngularMomC.AngularMomentum;
		const Eigen::Matrix3f& R = MassTensorC.MassTensor;
		const Eigen::Matrix3f GradPotential = GradPotenntialC.GradPotential;

		Eigen::Matrix3f QNext = Eigen::Matrix3f::Zero();
		Eigen::Matrix3f QLagrangeMultiplers = Eigen::Matrix3f::Zero();
		Eigen::Matrix3f PNext = Eigen::Matrix3f::Zero();

		//Some of these parameters are InOut so order of function calls matter
		RattleAngularPosition(QCurrent, PCurrent, R, GradPotential, QNext, QLagrangeMultiplers);
		RattleAngularMomentum(QCurrent, QNext, PCurrent, R, GradPotential, QLagrangeMultiplers, PNext);
	
		//Update physics state to next timestep
		//Annoying Eigen API. Why can't I just set the rotation?
		const Eigen::Vector3f& CoMTranslation = TransC.Transform.translation();
		TransC.Transform = Eigen::Affine3f().rotate(QNext).translate(CoMTranslation);
		AngularMomC.AngularMomentum = PNext;
	}
}