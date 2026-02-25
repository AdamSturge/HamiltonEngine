#pragma once

namespace HamiltonEngine::Physics 
{
	struct ParticleStateComponent 
	{
		ParticleStateComponent(float M, const Eigen::Vector3f& Pos,
			const Eigen::Vector3f& LinMom);

		float Mass; 
		Eigen::Vector3f Position;
		Eigen::Vector3f LinearMomentum;
	};
	
	/*struct PositionComponent
	{
		PositionComponent(const Eigen::Vector3f& Pos);
		Eigen::Vector3f Position;
	};
	
	struct LinearMomentumComponent
	{
		LinearMomentumComponent(const Eigen::Vector3f& Mom);
		Eigen::Vector3f LinearMomentum;
	};

	struct MassComponent
	{
		MassComponent(float M);
		float Mass;
	};*/
}