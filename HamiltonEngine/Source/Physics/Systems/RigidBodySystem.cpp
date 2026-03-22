#include "PrecompiledHeader/Pch.h"

#include "Physics/Systems/RigidBodySystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Integrators/RigidBodyB.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<int> NumRigidBodies("NumRigidBodies", 10);

	void CreateRigidBodyEntities()
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		std::vector< Eigen::Affine3f> Transforms(NumRigidBodies, Eigen::Affine3f::Identity());
		for (int EntityIndex = 0; EntityIndex < NumRigidBodies; ++EntityIndex) 
		{
			Transforms[EntityIndex].translate(Eigen::Vector3f(0.0f, EntityIndex * 2.0f, 0.0f));
		}

		//A rigid body is a particle with orientation, inertia tensor, and angular momentum
		for (int EntityIndex = 0; EntityIndex < NumRigidBodies; ++EntityIndex)
		{
			entt::entity RigidBodyEntity = Globals::Registry.create();

			Physics::RigidBodyStateComponent& RigidBodyState = 
				Globals::Registry.emplace<Physics::RigidBodyStateComponent>(RigidBodyEntity,
					RigidBodyStateComponent
					{
						Transforms[EntityIndex], //transform
						1.0f, // mass
						Eigen::Vector3f(0.0f,0.0f,0.0f), // linear momentum
						Eigen::Diagonal3f(1.0f, 1.0f, 1.0f), // inertial tensor
						Eigen::Vector3f(0.0f,0.0f,0.0f) // angular momentum
					});


			entt::entity GravityEntity = Globals::Registry.create();
			RigidBodyGravityComponent& GravityComponent = Globals::Registry.emplace<RigidBodyGravityComponent>(GravityEntity,
				RigidBodyGravityComponent{
					entt::const_handle(Globals::Registry,RigidBodyEntity) //Rigid body parent
				});

			RigidBodyState.PotentialEnergyListHead = entt::const_handle(Globals::Registry, GravityEntity);
		}

		{
			//Add spring between rigid body 0 and rigid body 1
			auto RigidBodyView = Globals::Registry.view<Physics::RigidBodyStateComponent>();
			if (RigidBodyView.size() >= 2) 
			{
				const entt::entity RigidBody0 = RigidBodyView.front();
				const entt::entity RigidBody1 = RigidBodyView.back();

				const RigidBodyStateComponent& State0 = Globals::Registry.get<RigidBodyStateComponent>(RigidBody0);
				const RigidBodyStateComponent& State1 = Globals::Registry.get<RigidBodyStateComponent>(RigidBody1);
			
				entt::entity SpringEntity0 = Globals::Registry.create();
				Physics::SpringPotentialComponent& SpringComponent0 = Globals::Registry.emplace<Physics::SpringPotentialComponent>(SpringEntity0,
					SpringPotentialComponent
					{
						entt::const_handle(Globals::Registry,RigidBody0), // Rigid body parent
						1.0f, // Spring constant
						0.0f, // Rest length 
						Eigen::Vector3f(0.0f,0.0f,0.0f) //Anchor point in body coordiantes
					});

				entt::entity SpringEntity1 = Globals::Registry.create();
				Physics::SpringPotentialComponent& SpringComponent1 = Globals::Registry.emplace<Physics::SpringPotentialComponent>(SpringEntity1,
					SpringPotentialComponent
					{
						entt::const_handle(Globals::Registry,RigidBody1), // Rigid body parent
						1.0f, // Spring constant
						0.0f, // Rest length 
						Eigen::Vector3f(0.0f,0.0f,0.0f) //Anchor point in body coordiantes
					});

				SpringComponent0.OtherEntity = entt::const_handle(Globals::Registry, SpringEntity1);
				SpringComponent1.OtherEntity = entt::const_handle(Globals::Registry, SpringEntity0);

				RigidBodyGravityComponent& Gravity0 = Globals::Registry.get<RigidBodyGravityComponent>(State0.PotentialEnergyListHead);
				RigidBodyGravityComponent& Gravity1 = Globals::Registry.get<RigidBodyGravityComponent>(State1.PotentialEnergyListHead);

				Gravity0.NextEntity = entt::const_handle(Globals::Registry,SpringEntity0);
				Gravity1.NextEntity = entt::const_handle(Globals::Registry, SpringEntity1);
			}
			
		}

	}

	void RigidBodySystem()
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//TODO look into EnTT groups instead of multi views

		//Rigid Body Sim
		auto RigidBodyView = Globals::Registry.view<Physics::RigidBodyStateComponent>();

		for (auto [Entity, StateC] : RigidBodyView.each())
		{
			RigidBodyB(StateC.Mass,
				StateC.LinearMomentum,
				StateC.InertiaTensor,
				StateC.Transform,
				StateC.AngularMomentum,
				StateC.PotentialEnergyListHead);

			//std::cout << StateC.Transform.translation() << std::endl << std::endl;
			//std::cout << StateC.Transform.rotation() << std::endl << std::endl;
		}
	}
}