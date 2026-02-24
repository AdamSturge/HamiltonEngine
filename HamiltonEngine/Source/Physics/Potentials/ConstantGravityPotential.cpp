#include "PrecompiledHeader/Pch.h"

#include "ConstantGravityPotential.h"
#include "Physics/State/ParticleState.h"
#include "Configuration/ConfigurationVariable.h"

namespace HamiltonEngine::Physics
{
	ConfigurationVariable<float> GravitationalAcceleration("GravitationalAcceleration", 9.8f);
	
	float ComputeConstantGravityPotential(const Eigen::Vector3f& Pos,
		float Mass)
	{
		const float Height = Pos.z();
		return Mass * GravitationalAcceleration * Height;
	}

	void ComputeGradConstantGravityPotential(float Mass,
		Eigen::Vector3f& OutGradPotentialEnergy)
	{
		OutGradPotentialEnergy += Mass * GravitationalAcceleration * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	}

	float ComputeConstantGravityPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition,
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor)
	{
		const Eigen::Vector3f WorldPosition = BodyToWorldTransform * BodyPosition;
		const float Height = WorldPosition.z();
		return Mass * GravitationalAcceleration * Height;
	}

	void ComputeGradConstantGravityPotentialRigidBody(const Eigen::Affine3f& BodyToWorldTransform, 
		Eigen::Vector3f BodyPosition, 
		float Mass,
		const Eigen::Diagonal3f& InertiaTensor, 
		Eigen::Vector3f& OutGradLinearPotentialEnergy, 
		Eigen::Vector3f& OutGradAngularPotentialEnergy)
	{
		OutGradLinearPotentialEnergy += Mass * GravitationalAcceleration * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	}
}