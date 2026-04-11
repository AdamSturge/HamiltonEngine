#include <PrecompiledHeader/Pch.h>
#include "Utils.h"
#include <Configuration/Globals.h>
#include "OpenGL.h"
#include "BasicShapes/Shapes.h"
#include "BasicShapes/Cube.h"

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

void createTestObjects()
{
	// This should be a Affine3f Matrix at some point in the future, but conceptually this makes it easier for Joel's brain to process
	HamiltonEngine::OpenGL::TransformComponent SimpleTransform;
	SimpleTransform.Scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	SimpleTransform.Position = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	SimpleTransform.RotationAxis = Eigen::Vector3f(1.0f, 0.0f, 0.0f).normalized();
	SimpleTransform.RotationAngle = 0;

	entt::registry& Reg = HamiltonEngine::Globals::Registry;
	// Some Test Objects
	auto ent = HamiltonEngine::OpenGL::CreateSphere(SimpleTransform, 3, 36, 36);
	HamiltonEngine::Globals::PrimativesBuffers["sphere"] = Reg.get<HamiltonEngine::OpenGL::OpenGLBuffersComponent>(ent);
	//Reg.get<OpenGLBuffersComponent>(ent);

	SimpleTransform.Position = Eigen::Vector3f(0.0f, 5.0f, 0.0f);
	ent = HamiltonEngine::OpenGL::CreateCube(SimpleTransform);
	HamiltonEngine::Globals::PrimativesBuffers["cube"] = Reg.get<HamiltonEngine::OpenGL::OpenGLBuffersComponent>(ent);

	SimpleTransform.Position = Eigen::Vector3f(0.0f, 10.0f, 0.0f);
	ent = HamiltonEngine::OpenGL::CreateTetra(SimpleTransform);
	HamiltonEngine::Globals::PrimativesBuffers["tetra"] = Reg.get<HamiltonEngine::OpenGL::OpenGLBuffersComponent>(ent);
}