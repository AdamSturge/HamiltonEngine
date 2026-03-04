#pragma once
#include "PrecompiledHeader/Pch.h"

namespace HamiltonEngine::OpenGL
{
	// CAMERA DEFAULTS
	const Eigen::Vector3f DEFAULT_CAMERA_POSITION(-10.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_FRONT(1.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_UP(0.0f, 0.0f, 1.0f); // Up is +Z
	const float DEFAULT_CAMERA_YAW = -180.0f;
	const float DEFAULT_CAMERA_PITCH = 0.0f + 90.0f;
	const float DEFAULT_FOV = 30;
	const float DEFAULT_NEAR_CLIP = 0.01f;
	const float DEFAULT_FAR_CLIP = 10.0f;

	struct Camera
		{
			Eigen::Vector3f CameraPosition;
			Eigen::Vector3f CameraFront;
			Eigen::Vector3f CameraUp;
			Eigen::Vector3f CameraRight; // Useful so we don't have to contintually calculate the X-product.
			Eigen::Vector3f WorldUp;
			float yaw;
			float pitch;
			// float roll;
			float fov;
		};

	Eigen::Matrix4f MakeFrustum(float fovY, float aspectRatio, float front, float back);
	Eigen::Matrix4f LookAt(Eigen::Vector3f CameraPos,Eigen::Vector3f TargetPos,Eigen::Vector3f Up);
	Eigen::Matrix4f CameraLookAt(HamiltonEngine::OpenGL::Camera& Camera, Eigen::Vector3f Target);
	void ProcessMouseMovement(HamiltonEngine::OpenGL::Camera& Camera, float DeltaX, float DeltaY);
	void ProcessKeyboardMovement(HamiltonEngine::OpenGL::Camera& Camera);
	void UpdateCameraVectors(HamiltonEngine::OpenGL::Camera& Camera);
	void PrintCameraDetails(const Camera& camera);

}