#include "PrecompiledHeader/Pch.h"
#include "Camera.h"
#include <math.h>

float DegToRad(float deg)
{
    return static_cast<float>(deg * (M_PI / 180));
}

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
	matrix(3,2) = -1;
	matrix(2,3) = -(2 * back * front) / (back - front);
	return matrix;
}

//Eigen::Affine3f lookAt = glm::lookAt(eye, centre, up)
Eigen::Matrix4f LookAt(
	Eigen::Vector3f CameraPos,
	Eigen::Vector3f TargetPos,
	Eigen::Vector3f Up
)
{

	Eigen::Vector3f CameraDirection = (CameraPos - TargetPos).normalized();
	Eigen::Vector3f CameraRight = Up.cross(CameraDirection).normalized();
	Eigen::Vector3f CameraUp = CameraDirection.cross(CameraRight);

	Eigen::Matrix4f LookingAt;

	// This can probably be simplified
	LookingAt << CameraRight.x(), CameraRight.y(), CameraRight.z(), 0.0f,
				CameraUp.x(), CameraUp.y(), CameraUp.z(), 0.0f,
				CameraDirection.x(), CameraDirection.y(), CameraDirection.z(), 0.0f,
				0, 0, 0, 1;

	Eigen::Matrix4f PosM4;
	PosM4 << 1, 0, 0, -CameraPos.x(),
			0, 1, 0, -CameraPos.y(),
			0, 0, 1, -CameraPos.z(),
			0, 0, 0, 1;
	
	Eigen::Matrix4f res = LookingAt * PosM4;

	return res;
}