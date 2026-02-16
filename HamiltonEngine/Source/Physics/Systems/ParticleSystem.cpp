#include "PrecompiledHeader/Pch.h"

#include "ParticleSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/Integrators/EulerB.h"
#include "Physics/Integrators/StormerVerlet.h"

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
		auto ParticleView = Registry.view<
			Physics::MassComponent,
			Physics::PositionComponent,
			Physics::LinearMomentumComponent>();

		for (auto [Entity, MassC, PositionC, LinMomC] : ParticleView.each())
		{
			//EulerB(MassC.Mass, PositionC.Position, LinMomC.LinearMomentum);
			StormerVerlet(MassC.Mass, PositionC.Position, LinMomC.LinearMomentum);
			//std::cout << PositionC.Position << std::endl << std::endl;
		}
	}
}