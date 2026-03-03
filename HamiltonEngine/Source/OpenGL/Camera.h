#pragma once
#include "PrecompiledHeader/Pch.h"

namespace HamiltonEngine::OpenGL
{
	// CAMERA DEFAULTS
	const Eigen::Vector3f DEFAULT_CAMERA_POSITION(-10.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_FRONT(1.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_UP(0.0f, 0.0f, 1.0f);
	const float DEFAULT_CAMERA_YAW = -90.0f;
	const float DEFAULT_CAMERA_PITCH = 0.0f;


	struct Camera
		{
			Eigen::Vector3f CameraPosition;
			Eigen::Vector3f CameraFront;
			Eigen::Vector3f CameraUp;
			float yaw;
			float pitch;
			// float roll;
			float fov;
		};

	Eigen::Matrix4f MakeFrustum(float fovY, float aspectRatio, float front, float back);
	Eigen::Matrix4f LookAt(Eigen::Vector3f CameraPos,Eigen::Vector3f TargetPos,Eigen::Vector3f Up);
	void PrintCameraDetails(const Camera& camera);

}