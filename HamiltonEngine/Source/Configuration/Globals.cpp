#include "PrecompiledHeader/Pch.h"

#include "Globals.h"


namespace HamiltonEngine::Globals 
{
	ConfigurationVariable<float> PhysicsTickLength = ConfigurationVariable<float>("PhysicsTickLength", 1.0f / 60.0f);
	ConfigurationVariable<bool> PhysicsSimEnabled = ConfigurationVariable<bool>("PhysicsSimEnabled", true);
	ConfigurationVariable<float> Epsilon = ConfigurationVariable<float>("Epsilon", 1e-4f);


	// Rendering + OpenGL stuff
	GLuint FrameCount = 0;

	//EnTT
	entt::registry Registry = entt::registry();

	// Control States
	HamiltonEngine::OpenGL::Camera Camera{
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION, 
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT, 
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP,
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW, 
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH, 
		// Pitch if we need it
		HamiltonEngine::OpenGL::DEFAULT_FOV};

}
