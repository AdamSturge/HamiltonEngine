#include "PrecompiledHeader/Pch.h"
#include "Camera.h"
#include "Utils.h"
#include <Configuration/ConfigurationVariable.h>
#include "Configuration/Globals.h"

namespace HamiltonEngine::OpenGL
{
	// CAMERA DEFAULTS
	const Eigen::Vector3f DEFAULT_CAMERA_POSITION(-25.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_FRONT(1.0f, 0.0f, 0.0f);
	const Eigen::Vector3f DEFAULT_CAMERA_UP(0.0f, 0.0f, 1.0f); // Up is +Z
	const float DEFAULT_CAMERA_YAW = 0.0f;
	const float DEFAULT_CAMERA_PITCH = 0.0f;
	const float DEFAULT_FOV = 30;
	const float DEFAULT_NEAR_CLIP = 0.01f;
	const float DEFAULT_FAR_CLIP = 10.0f;


	Eigen::Matrix4f MakeFrustum(float fovY, float aspectRatio, float front, float back)
	{
		// basically stolen from: https://www.songho.ca/opengl/gl_projectionmatrix.html#fov
		float tangent = tan(DegToRad(fovY) / 2);
		float top = front * tangent;
		float right = top * aspectRatio;

		Eigen::Matrix4f matrix = Eigen::Matrix4f::Zero();

		matrix(0, 0) = front / right;
		matrix(1, 1) = front / top;
		matrix(2, 2) = -(back + front) / (back - front);
		matrix(3, 2) = -1;
		matrix(2, 3) = -(2 * back * front) / (back - front);
		return matrix;
	}

	Eigen::Matrix4f LookAt(Eigen::Vector3f CameraPos, Eigen::Vector3f TargetPos,Eigen::Vector3f Up)
	{
		Eigen::Vector3f CameraDirection = (CameraPos - TargetPos).normalized();
		Eigen::Vector3f CameraRight = Up.cross(CameraDirection).normalized();
		Eigen::Vector3f CameraUp = CameraDirection.cross(CameraRight);

		Eigen::Matrix4f LookingAt;

		// This can probably be simplified
		LookingAt << CameraRight.x(),	  CameraRight.y(),     CameraRight.z(), 0.0f,
						CameraUp.x(),	 	 CameraUp.y(),		  CameraUp.z(),	0.0f,
				 CameraDirection.x(), CameraDirection.y(), CameraDirection.z(), 0.0f,
								   0,				    0,				     0,    1;

		Eigen::Matrix4f PosM4;
		PosM4 << 1, 0, 0, -CameraPos.x(),
				 0, 1, 0, -CameraPos.y(),
				 0, 0, 1, -CameraPos.z(),
				 0, 0, 0, 1;

		Eigen::Matrix4f res = LookingAt * PosM4;

		return res;
	}

	Eigen::Matrix4f CameraLookAt(HamiltonEngine::OpenGL::Camera& Camera, Eigen::Vector3f Target)
	{
		Eigen::Vector3f f = (Target - Camera.CameraPosition).normalized();
		Eigen::Vector3f u = Camera.WorldUp.normalized();
		Eigen::Vector3f s = f.cross(u).normalized();
		u = s.cross(f);

		Eigen::Matrix4f res;
		res << s.x(), s.y(), s.z(), -s.dot(Camera.CameraPosition),
			   u.x(), u.y(), u.z(), -u.dot(Camera.CameraPosition),
			   -f.x(), -f.y(), -f.z(),  f.dot(Camera.CameraPosition),
			   0,0,0,1;

		return res;
	}

	void ProcessMouseMovement(HamiltonEngine::OpenGL::Camera& Camera, float DeltaX, float DeltaY)
	{
		float MouseSensitivity = HamiltonEngine::ConfigurationVariable<float>("MouseSensitivity", 0.1f);
		DeltaX *= MouseSensitivity;
		DeltaY *= MouseSensitivity;

		Camera.yaw -= DeltaX;
		Camera.pitch += DeltaY;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (Camera.pitch > 89.0f)
			Camera.pitch = 89.0f;
		if (Camera.pitch < -89.0f)
			Camera.pitch = -89.0f;

		UpdateCameraVectors(Camera);

	}

	void ProcessKeyboardMovement(GLFWwindow* Window, HamiltonEngine::OpenGL::Camera& Camera, CameraDirection dir, float DeltaTime)
	{

		//HamiltonEngine::OpenGL::Camera& camera = HamiltonEngine::Globals::ActiveCamera;
		const float CameraSpeed = HamiltonEngine::ConfigurationVariable<float>("MovementSpeed", 2.5) * DeltaTime;

		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Camera.CameraPosition += CameraSpeed * Camera.CameraFront;
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Camera.CameraPosition -= CameraSpeed * Camera.CameraFront;
		}

		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Camera.CameraPosition -= Camera.CameraFront.cross(Camera.CameraUp).normalized() * CameraSpeed;

		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Camera.CameraPosition += Camera.CameraFront.cross(Camera.CameraUp).normalized() * CameraSpeed;
		}
	}

	void UpdateCameraVectors(HamiltonEngine::OpenGL::Camera& Camera)
	{

		Eigen::Vector3f NewFront;

		NewFront = { cos(DegToRad(Camera.pitch)) * cos(DegToRad(Camera.yaw)),
					 sin(DegToRad(Camera.yaw)) * cos(DegToRad(Camera.pitch)),
					 sin(DegToRad(Camera.pitch))
					};


		NewFront.normalize();
		Camera.CameraFront = NewFront;
		
		Eigen::Vector3f Right = Camera.CameraFront.cross(Camera.WorldUp).normalized();
		Camera.CameraUp = Right.cross(Camera.CameraFront).normalized();

	}

	void PrintCameraDetails(const Camera& camera)
	{
		printf("Frame - %d\n", HamiltonEngine::Globals::FrameCount);
		printf("Camera Location - ( %f, %f, %f )\n", camera.CameraPosition.x(), camera.CameraPosition.y(), camera.CameraPosition.z());
		printf("Camera YPR - ( %f, %f, %f )\n", camera.yaw, camera.pitch, 0.0f);
		printf("Camera Front - ( %f, %f, %f )\n", camera.CameraFront.x(), camera.CameraFront.y(), camera.CameraFront.z());
		printf("Camera Up - ( %f, %f, %f )\n", camera.CameraUp.x(), camera.CameraUp.y(), camera.CameraUp.z());
	}
}