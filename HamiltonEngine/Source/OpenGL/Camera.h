#pragma once
#include "PrecompiledHeader/Pch.h"

namespace HamiltonEngine::OpenGL
{
	struct Camera
		{
			Eigen::Vector3f CameraPosition;
			Eigen::Vector3f CameraFront;
			Eigen::Vector3f CameraUp;
			float yaw;
			float pitch;
			float fov;
		};

	Eigen::Matrix4f MakeFrustum(float fovY, float aspectRatio, float front, float back);
	Eigen::Matrix4f LookAt(Eigen::Vector3f CameraPos,Eigen::Vector3f TargetPos,Eigen::Vector3f Up);

}