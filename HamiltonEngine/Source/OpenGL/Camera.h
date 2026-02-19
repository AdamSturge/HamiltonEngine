#pragma once
#include "PrecompiledHeader/Pch.h"

namespace HamiltonEngine::OpenGL
{
	struct Camera
		{
			Eigen::Vector3f CameraPosition;
			Eigen::Vector3f CameraFront;
			Eigen::Vector3f CameraUp;
		};


	//Eigen::Matrix4f LookAt(
	//	Eigen::Vector3f CameraPos,
	//	Eigen::Vector3f TargetPos,
	//	Eigen::Vector3f Up
	//)
	//{

	//	Eigen::Vector3f CameraDirection = (CameraPos - TargetPos).normalized();
	//	Eigen::Vector3f CameraRight = Up.cross(CameraDirection).normalized();
	//	Eigen::Vector3f CameraUp = CameraDirection.cross(CameraRight);

	//	Eigen::Matrix4f LookingAt;

	//	// This can probably be simplified
	//	LookingAt << CameraRight.x(), CameraRight.y(), CameraRight.z(), 0.0f,
	//		CameraUp.x(), CameraUp.y(), CameraUp.z(), 0.0f,
	//		CameraDirection.x(), CameraDirection.y(), CameraDirection.z(), 0.0f,
	//		0, 0, 0, 1;

	//	Eigen::Matrix4f PosM4;
	//	PosM4 << 1, 0, 0, -CameraPos.x(),
	//		0, 1, 0, -CameraPos.y(),
	//		0, 0, 1, -CameraPos.z(),
	//		0, 0, 0, 1;

	//	Eigen::Matrix4f res = LookingAt * PosM4;

	//	return res;
	//}
}