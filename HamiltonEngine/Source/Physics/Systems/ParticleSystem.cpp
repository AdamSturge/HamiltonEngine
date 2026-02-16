#include "PrecompiledHeader/Pch.h"

#include "ParticleSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/Integrators/HamiltonianSplitting.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<int> NumParticles("NumParticles", 10);
	ConfigurationVariable<int> NumRigidBodies("NumRigidBodies", 10);
	
	void CreateParticlesEntities(entt::registry& Registry)
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}
		
		for (int EntityIndex = 0; EntityIndex < NumParticles; ++EntityIndex)
		{
			entt::entity Entity = Registry.create();

			Registry.emplace<Physics::PositionComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::LinearMomentumComponent>(Entity, Eigen::Vector3f(1.0f, 0.0f, 0.0f));
			Registry.emplace<Physics::MassComponent>(Entity, 1.0f);
			Registry.emplace<Physics::GradParticlePotentialComponent>(Entity, Eigen::Vector3f::Zero());
		}
		
	}

	void ParticleSystem(entt::registry& Registry) 
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//TODO look into EnTT groups instead of multi views

		//Particle Sim
		auto SymplecticEulerPhysicsSimView = Registry.view<
			Physics::MassComponent,
			Physics::PositionComponent,
			Physics::LinearMomentumComponent>();

		for (auto [Entity, MassC, PositionC, LinMomC] : SymplecticEulerPhysicsSimView.each())
		{
			////This should be Stormer-Verlet
			//constexpr int N = 2;
			//const Physics::EulerModePair Modes[2]{
			//	{ Physics::EulerMode::PotentialOnly, Physics::EulerMode::KineticOnly },
			//	{ Physics::EulerMode::None, Physics::EulerMode::PotentialOnly } };
			//const float A[N]{ 1.0f, 1.0f }; //Potential weights
			//const float B[N]{ 1.0f, 0.0f }; //Kinetic weights
			//const float C[N]{ 0.5f, 0.5f }; //Potential time substep
			//const float D[N]{ 1.0f, 1.0f }; //Kinetic time substep
			//Physics::HamiltonianSplittingSystem<N>(A, B, C, D, Modes, MassC, PositionC, LinMomC);

			//std::cout << PositionC.Position << std::endl << std::endl;
		}
	}
}