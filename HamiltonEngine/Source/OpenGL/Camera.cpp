#include "PrecompiledHeader/Pch.h"
#include "Camera.h"
#include "Utils.h"
#include <Configuration/ConfigurationVariable.h>
#include "Configuration/Globals.h"

namespace HamiltonEngine::OpenGL
{

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

	//Eigen::Affine3f lookAt = glm::lookAt(eye, centre, up)
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

		//LookingAt = { {}, {}, {}, {} };


		Eigen::Matrix4f PosM4;
		PosM4 << 1, 0, 0, -CameraPos.x(),
				 0, 1, 0, -CameraPos.y(),
				 0, 0, 1, -CameraPos.z(),
				 0, 0, 0, 1;

		Eigen::Matrix4f res = LookingAt * PosM4;



		//Eigen::Vector3f zaxis = (CameraPos - TargetPos).normalized();
		//Eigen::Vector3f xaxis = ()


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

		Camera.yaw += DeltaX;
		Camera.pitch += DeltaY;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (Camera.pitch > 89.0f + 90.0f)
			Camera.pitch = 89.0f + 90.0f;
		if (Camera.pitch < -89.0f - 90.0f)
			Camera.pitch = -89.0f - 90.0f;

		UpdateCameraVectors(Camera);

	}

	void ProcessKeyboardMovement(HamiltonEngine::OpenGL::Camera& Camera)
	{

		// TODO
	}

	void UpdateCameraVectors(HamiltonEngine::OpenGL::Camera& Camera)
	{

		Eigen::Vector3f NewFront;
		/*NewFront = { 
					 
					 
		};*/

		// UE5 ?
		//NewFront = { -1 * sin(DegToRad(Camera.yaw)) * cos(DegToRad(Camera.pitch)),
		//			cos(DegToRad(Camera.pitch)) * cos(DegToRad(Camera.yaw)),
		//			sin(DegToRad(Camera.pitch)),
		//			};

		NewFront << cos(DegToRad(Camera.yaw)) * cos(DegToRad(Camera.pitch)),
					sin(DegToRad(Camera.pitch)),
					sin(DegToRad(Camera.yaw))* cos(DegToRad(Camera.pitch));

		//NewFront = { sin(DegToRad(Camera.pitch)) * cos(DegToRad(Camera.yaw)),
		//			 sin(DegToRad(Camera.yaw)),
		//			 cos(DegToRad(Camera.pitch)) * cos(DegToRad(Camera.yaw)) };


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