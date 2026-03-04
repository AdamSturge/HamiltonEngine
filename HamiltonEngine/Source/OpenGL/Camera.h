#pragma once
#include "PrecompiledHeader/Pch.h"


namespace HamiltonEngine::OpenGL
{
	enum CameraDirection { FORWARD, BACKWARD, LEFT, RIGHT };

	extern const Eigen::Vector3f DEFAULT_CAMERA_POSITION;
	extern const Eigen::Vector3f DEFAULT_CAMERA_FRONT;
	extern const Eigen::Vector3f DEFAULT_CAMERA_UP;
	extern const float DEFAULT_CAMERA_YAW;
	extern const float DEFAULT_CAMERA_PITCH;
	extern const float DEFAULT_FOV;
	extern const float DEFAULT_NEAR_CLIP;
	extern const float DEFAULT_FAR_CLIP;

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
	void ProcessKeyboardMovement(GLFWwindow* Window, HamiltonEngine::OpenGL::Camera& Camera, CameraDirection dir, float DeltaTime);
	void UpdateCameraVectors(HamiltonEngine::OpenGL::Camera& Camera);
	void PrintCameraDetails(const Camera& camera);

}