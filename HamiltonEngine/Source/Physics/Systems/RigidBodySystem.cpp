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

			Registry.emplace<Physics::PositionComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::LinearMomentumComponent>(Entity, Eigen::Vector3f(1.0f, 0.0f, 0.0f));
			Registry.emplace<Physics::MassComponent>(Entity, 1.0f);
			Registry.emplace<Physics::OrientationComponent>(Entity, Eigen::Matrix3f::Identity());
			Registry.emplace<Physics::AngularMomentumComponent>(Entity, Eigen::Vector3f::Zero());
			Registry.emplace<Physics::InertiaTensorComponent>(Entity, Eigen::Diagonal3f(1.0f, 1.0f, 1.0f));
			Registry.emplace<Physics::RigidBodyPotentialEnergyListHeadComponent>(Entity);
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
		auto RigidBodyView = Registry.view<
			InertiaTensorComponent,
			OrientationComponent,
			AngularMomentumComponent>();

		for (auto [Entity, InertiaC, OrientationC, AngMomC] : RigidBodyView.each())
		{
			constexpr int NumPotential = 1;
			constexpr int NumKinetic = 3;
			constexpr float PotentialWeights[NumPotential]{ 1.0f };
			constexpr float KineticWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
			constexpr float PotentialTickRateWeights[NumPotential]{ 1.0f };
			constexpr float KineticTickRateWeights[NumKinetic]{ 1.0f,1.0f,1.0f };
			constexpr int PotentialIndex = 0;
			constexpr int KineticIndex = 0;
			

			RigidBodyFlowComposition<NumPotential, NumKinetic,
				PotentialIndex, KineticIndex,
				RigidBodyIntegrationCompositionMode::KineticX,
				RigidBodyIntegrationCompositionMode::KineticY,
				RigidBodyIntegrationCompositionMode::KineticZ,
				RigidBodyIntegrationCompositionMode::Potential>(PotentialWeights,
					KineticWeights, 
					PotentialTickRateWeights, 
					KineticTickRateWeights, 
					InertiaC.InertiaTensor, 
					OrientationC.Orientation,
					AngMomC.AngularMomentum);
			
			//RigidBodyKineticXOnly(InertiaC.InertiaTensor, OrientationC.Orientation, AngMomC.AngularMomentum);
			//std::cout << OrientationC.Orientation.transpose() * OrientationC.Orientation << std::endl << std::endl;
			//std::cout << AngMomC.AngularMomentum.norm() << std::endl << std::endl;
		}
	}
}