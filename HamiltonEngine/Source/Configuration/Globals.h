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
//	extern ConfigurationVariable<int> WindowHeight;
//	extern ConfigurationVariable<int> WindowWidth;
//	extern ConfigurationVariable<std::string> WindowName;
	extern ConfigurationVariable<float> FieldOfView;
	extern ConfigurationVariable<float> NearClipPlane;
	extern ConfigurationVariable<float> FarClipPlane;
	extern GLuint FrameCount;

	//EnTT
	extern entt::registry Registry;

	// Control States
	extern HamiltonEngine::OpenGL::Camera Camera;
}