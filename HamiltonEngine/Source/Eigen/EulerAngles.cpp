#include "PrecompiledHeader/Pch.h"
#include "EulerAngles.h"

namespace HamiltonEngine 
{
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::EulerAngles& Angles, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("Pitch", Angles.Pitch));
		Record(cereal::make_nvp("Yaw", Angles.Yaw));
		Record(cereal::make_nvp("Roll", Angles.Roll));
	}

	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::EulerAngles& Angles, const std::uint32_t Version)
	{
	}

	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, EulerAngles& EulerAngles)
	{
		float Angles[3]{ 0.0f, 0.0f, 0.0f };
		RotationToEulerAngles(Rotation, Angles);
		EulerAngles.Pitch = Angles[0];
		EulerAngles.Yaw = Angles[1];
		EulerAngles.Roll = Angles[2];
	}

	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, float Angles[3])
	{
		Angles = Rotation.canonicalEulerAngles(0, 1, 2).data(); //X,Y,Z ordering.
	}

	void RotationToEulerAngles(const Eigen::Matrix3f& Rotation, float& Pitch, float& Yaw, float& Roll)
	{
		float Angles[3]{ 0.0f, 0.0f, 0.0f };
		RotationToEulerAngles(Rotation, Angles);
		Pitch = Angles[0];
		Yaw = Angles[0];
		Roll = Angles[0];
	}

	void EulerAnglesToRotation(const float Pitch, const float Yaw, const float& Roll, Eigen::Matrix3f& Rotation)
	{
		Eigen::AngleAxisf PitchAngle(Pitch, Eigen::Vector3f::UnitX());
		Eigen::AngleAxisf YawAngle(Yaw, Eigen::Vector3f::UnitY());
		Eigen::AngleAxisf RollAngle(Roll, Eigen::Vector3f::UnitZ());

		Eigen::Quaternion<float> Q = RollAngle * YawAngle * PitchAngle;

		Rotation = Q.matrix();
	}
}