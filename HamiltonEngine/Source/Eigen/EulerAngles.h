#pragma once

namespace HamiltonEngine
{
	struct EulerAngles
	{
		float Pitch = 0.0f;
		float Yaw = 0.0f;
		float Roll = 0.0f;
	};

	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::EulerAngles& Angles, const std::uint32_t Version);
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::EulerAngles& Angles, const std::uint32_t Version);
	
	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, EulerAngles& EulerAngles);
	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, float& Pitch, float& Yaw, float& Roll);
	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, float Angles[3]);
	void EulerAnglesToRotation(const float Pitch, const float Yaw, const float& Roll, Eigen::Matrix3f& Rotation);
}

CEREAL_CLASS_VERSION(HamiltonEngine::EulerAngles, 1);