#include "PrecompiledHeader/Pch.h"

#include "ParticleSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/Integrators/EulerA.h"
#include "Physics/Integrators/EulerB.h"
#include "Physics/Integrators/StormerVerlet.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<int> NumParticles("NumParticles", 10);

	void CreateParticleEntities()
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		for (int EntityIndex = 0; EntityIndex < NumParticles; ++EntityIndex)
		{
			entt::entity Entity = Globals::Registry.create();

			Globals::Registry.emplace<Physics::ParticleStateComponent>(Entity, ParticleStateComponent{1.0f, Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero()});
		}
	}

	void ParticleSystem()
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//TODO look into EnTT groups instead of multi views

		//Particle Sim
		auto ParticleView = Globals::Registry.view<ParticleStateComponent>();

		for (auto [Entity, ParticleStateC] : ParticleView.each())
		{
			//EulerA(MassC.Mass, PositionC.Position, LinMomC.LinearMomentum);
			EulerB(ParticleStateC.Mass, ParticleStateC.Position, ParticleStateC.LinearMomentum);
			//StormerVerlet(MassC.Mass, PositionC.Position, LinMomC.LinearMomentum);
			//std::cout << PositionC.Position << std::endl << std::endl;
		}
	}
}