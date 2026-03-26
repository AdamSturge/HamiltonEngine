#include <PrecompiledHeader/Pch.h>
#include "Utils.h"

float DegToRad(float deg)
{
    return static_cast<float>(deg * (M_PI / 180));
}

unsigned RandomRange(unsigned range)
{
	for (unsigned x, r;; )
	{
		if (x = rand(), r = x % range, x - r <= range)
		{
			return r;
		}
	}
}

HamiltonEngine::OpenGL::TransformComponent RandomTransformComponent()
{
	HamiltonEngine::OpenGL::TransformComponent Trans;

	float x, y, z, s;

	x = static_cast<float>(RandomRange(30));
	y = static_cast<float>(RandomRange(30));
	z = static_cast<float>(RandomRange(30));
	Eigen::Vector3f PosRot = Eigen::Vector3f(x, y, z);
	Trans.Position = PosRot;
	Trans.RotationAxis = PosRot;
	Trans.RotationAngle = static_cast<float>(RandomRange(360));
	Trans.RotationAngle = 0;

	s = static_cast<float>(RandomRange(5));
	Trans.Scale = Eigen::Vector3f(s, s, s);

	return Trans;
}

void PrintOpenGLBufferComponentData(HamiltonEngine::OpenGL::OpenGLBuffersComponent buff)
{
	printf("Buff Details:\nVAO ID: %d\nVBO ID: %d\nEBO: %d\nDraw Mode: %d\nStart: %d\nCount: %d\n", 
		buff.VAO, buff.VBO, buff.EBO, buff.DrawMode, buff.start, buff.count);

}

void PrintTransformComponent(HamiltonEngine::OpenGL::TransformComponent Trans)
{
	printf("Transform Component:\n");
	std::cout << "Position:\n" << Trans.Position << std::endl;
	std::cout << "RotationAxis:\n" << Trans.RotationAxis << std::endl;
	std::cout << "Scale:\n" << Trans.Scale << std::endl;
}