#pragma once
#include <Configuration/ConfigurationVariable.h>
#include <OpenGL/Camera.h>


namespace HamiltonEngine::Globals
{
	extern ConfigurationVariable<float> PhysicsTickLength;
	extern ConfigurationVariable<bool> PhysicsSimEnabled;

	// small number. Useful for comparison to near 0
	extern ConfigurationVariable<float> Epsilon;

	// Rendering + OpenGL stuff
	extern GLuint FrameCount;

	//EnTT
	extern entt::registry Registry;

	// Control States
	extern HamiltonEngine::OpenGL::Camera ActiveCamera;
}