#include "PrecompiledHeader/Pch.h"

#include "ParticleSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/Integrators/EulerA.h"
#include "Physics/Integrators/EulerB.h"
#include "Physics/Integrators/StormerVerlet.h"
#include "Physics/Potentials/ConstantGravityPotential.h"

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
			entt::entity ParticleEntity = Globals::Registry.create();

			ParticleStateComponent& ParticleState =
				Globals::Registry.emplace<ParticleStateComponent>(ParticleEntity,
					ParticleStateComponent
					{
						1.0f, //Mass
						Eigen::Vector3f::Zero(), //Position
						Eigen::Vector3f::Zero() // Linear Momentum
					});

			entt::entity GraivtyEntity = Globals::Registry.create();

			Globals::Registry.emplace<ParticleGravityComponent>(GraivtyEntity,
				ParticleGravityComponent{
					entt::const_handle(Globals::Registry, ParticleEntity)
				});

			ParticleState.PotentialEnergyListHead = entt::const_handle(Globals::Registry, GraivtyEntity);
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
			EulerB(ParticleStateC.Mass, 
				ParticleStateC.Position,
				ParticleStateC.LinearMomentum,
				ParticleStateC.PotentialEnergyListHead);
			//StormerVerlet(MassC.Mass, PositionC.Position, LinMomC.LinearMomentum);
			//std::cout << ParticleStateC.Position << std::endl << std::endl;
		}
	}
}