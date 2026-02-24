#include "PrecompiledHeader/Pch.h"

#include "Physics/Systems/RigidBodySystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Integrators/RigidBodyFlowComposition.h"
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
			Globals::Registry.emplace<RigidBodyPotentialEnergyComponent>(GravityEntity,
				RigidBodyPotentialEnergyComponent{ 
					entt::const_handle(Globals::Registry,RigidBodyEntity), //Rigid body parent
					Eigen::Vector3f::Zero(), // body coordinates for point of application
					&ComputeConstantGravityPotentialRigidBody,
					&ComputeGradConstantGravityPotentialRigidBody
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
			constexpr int NumPotential = 1;
			constexpr int NumKinetic = 3;
			constexpr float PotentialWeights[NumPotential]{ 1.0f };
			constexpr float KineticWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
			constexpr float PotentialTickRateWeights[NumPotential]{ 1.0f };
			constexpr float KineticTickRateWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
			constexpr int PotentialIndex = 0;
			constexpr int KineticIndex = 0;
			
			const RigidBodyPotentialEnergyComponent* PotentialEnergyComponent = nullptr;
			if (StateC.PotentialEnergyListHead.valid()) 
			{
				PotentialEnergyComponent = &StateC.PotentialEnergyListHead.get<RigidBodyPotentialEnergyComponent>();
			}

			RigidBodyFlowComposition<NumPotential, NumKinetic,
				PotentialIndex, KineticIndex,
				RigidBodyIntegrationCompositionMode::KineticX,
				RigidBodyIntegrationCompositionMode::KineticY,
				RigidBodyIntegrationCompositionMode::KineticZ,
				RigidBodyIntegrationCompositionMode::Potential>(PotentialWeights,
					KineticWeights, 
					PotentialTickRateWeights, 
					KineticTickRateWeights,
					StateC.Mass,
					StateC.LinearMomentum,
					StateC.InertiaTensor,
					StateC.Transform,
					StateC.AngularMomentum,
					PotentialEnergyComponent);

			//std::cout << StateC.Transform.translation() << std::endl << std::endl;
			//std::cout << StateC.Transform.rotation() << std::endl << std::endl;
		}
	}
}