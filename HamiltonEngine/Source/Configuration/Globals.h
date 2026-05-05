#pragma once
#include <Configuration/ConfigurationVariable.h>
#include <OpenGL/Camera.h>
#include <OpenGL/OpenGL.h>


namespace HamiltonEngine::Globals
{
	extern ConfigurationVariable<float> PhysicsTickLength;
	extern ConfigurationVariable<bool> PhysicsSimEnabled;

	// small number. Useful for comparison to near 0
	extern ConfigurationVariable<float> Epsilon;

	extern ConfigurationVariable<std::string> StartingLevel;

	// Rendering + OpenGL stuff
	extern GLuint FrameCount;
	extern std::map < std::string, HamiltonEngine::OpenGL::OpenGLBuffersComponent>  PrimativesBuffers;

	//EnTT
	extern entt::registry Registry;

	// Control States
	extern HamiltonEngine::OpenGL::Camera ActiveCamera;
}