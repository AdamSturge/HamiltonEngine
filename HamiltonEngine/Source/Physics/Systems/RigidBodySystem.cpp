#include "PrecompiledHeader/Pch.h"

#include "Physics/Systems/RigidBodySystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/State/ParticleState.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Integrators/RigidBodyFlowComposition.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<int> NumRigidBodies("NumRigidBodies", 10);

	void CreateRigidBodyEntities(entt::registry& Registry)
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//A rigid body is a particle with orientation, inertia tensor, and angular momentum
		for (int EntityIndex = 0; EntityIndex < NumRigidBodies; ++EntityIndex)
		{
			entt::entity Entity = Registry.create();

			Registry.emplace<Physics::RigidBodyStateComponent>(Entity, 
				RigidBodyStateComponent
				{
					Eigen::Affine3f::Identity(), //transform
					1.0f, // mass
					Eigen::Vector3f::Zero(), // linear momentum
					Eigen::Diagonal3f(1.0f, 1.0f, 1.0f), // inertial tensor
					Eigen::Vector3f::Zero() // angular momentum
				}); 
		}

	}

	void RigidBodySystem(entt::registry& Registry)
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}

		//TODO look into EnTT groups instead of multi views

		//Rigid Body Sim
		auto RigidBodyView = Registry.view<Physics::RigidBodyStateComponent>();

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
			

			const RigidBodyPotentialEnergyComponent& PotentialEnergyComponent = 
				StateC.PotentialEnergyListHead.get<RigidBodyPotentialEnergyComponent>();

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
			
			//RigidBodyKineticXOnly(InertiaC.InertiaTensor, OrientationC.Orientation, AngMomC.AngularMomentum);
			//std::cout << OrientationC.Orientation.transpose() * OrientationC.Orientation << std::endl << std::endl;
			//std::cout << AngMomC.AngularMomentum.norm() << std::endl << std::endl;
		}
	}
}