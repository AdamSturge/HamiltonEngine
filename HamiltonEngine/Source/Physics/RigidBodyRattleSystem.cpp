#include "PrecompiledHeader/Pch.h"

#include "RigidBodyRattleSystem.h"
#include "ParticleState.h"
#include "Physics/RigidBodyState.h"
#include "Configuration/Globals.h"

namespace HamiltonEngine::Physics 
{
	void RigidBodyRattleSystem(TransformComponent& TransC,
		AngularMomentumComponent& AngularMomC,
		const MassTensorComponent MassTensorC,
		const GradRigidBodyPotentialComponent& GradPotenntialC)
	{ 
		Eigen::Matrix3f Q = TransC.Transform.rotation();
		Eigen::Matrix3f RInv = MassTensorC.MassTensor.inverse();
		Eigen::Matrix3f QBar = Q 
			+ Globals::PhysicsTickLength * AngularMomC.AngularMomentum * RInv
			- 0.5f * Globals::PhysicsTickLength * Globals::PhysicsTickLength * GradPotenntialC.GradPotential * RInv;
	}
}