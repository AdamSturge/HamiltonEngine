#include "PrecompiledHeader/Pch.h"

#include "Physics/Systems/RigidBodySystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Integrators/RigidBodyB.h"
#include "Physics/Potentials/ConstantGravityPotential.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<int> NumRigidBodies("NumRigidBodies", 10);

	void CreateRigidBodyEntities()
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//A rigid body is a particle with orientation, inertia tensor, and angular momentum
		for (int EntityIndex = 0; EntityIndex < NumRigidBodies; ++EntityIndex)
		{
			entt::entity RigidBodyEntity = Globals::Registry.create();

			Physics::RigidBodyStateComponent& RigidBodyState = 
				Globals::Registry.emplace<Physics::RigidBodyStateComponent>(RigidBodyEntity,
					RigidBodyStateComponent
					{
						Eigen::Affine3f::Identity(), //transform
						1.0f, // mass
						Eigen::Vector3f(0.0f,0.0f,0.0f), // linear momentum
						Eigen::Diagonal3f(1.0f, 1.0f, 1.0f), // inertial tensor
						Eigen::Vector3f(0.0f,0.0f,0.0f) // angular momentum
					});


			entt::entity GravityEntity = Globals::Registry.create();
			Globals::Registry.emplace<RigidBodyGravityComponent>(GravityEntity,
				RigidBodyGravityComponent{
					entt::const_handle(Globals::Registry,RigidBodyEntity) //Rigid body parent
				});

			RigidBodyState.PotentialEnergyListHead = entt::const_handle(Globals::Registry, GravityEntity);
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