#include "PrecompiledHeader/Pch.h"

#include "Globals.h"


namespace HamiltonEngine::Globals 
{
	ConfigurationVariable<float> PhysicsTickLength = ConfigurationVariable<float>("PhysicsTickLength", 1.0f / 60.0f);
	ConfigurationVariable<bool> PhysicsSimEnabled = ConfigurationVariable<bool>("PhysicsSimEnabled", true);
	ConfigurationVariable<float> Epsilon = ConfigurationVariable<float>("Epsilon", 1e-4f);


	// Rendering + OpenGL stuff
	ConfigurationVariable<int> NewNameWindowHeight("WindowHeight", 800);
	ConfigurationVariable<int> INeverUseThisNewNameWindowWidth("WindowWidth", 600); // Removing this line breaks it?
	//ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");
	ConfigurationVariable<float> FieldOfView("FieldOfView", 30.0);
	ConfigurationVariable<float> NearClipPlane("NearClipPlane", 0.01f);
	ConfigurationVariable<float> FarClipPlane("FarClipPlane", 2000.0f);
	GLuint FrameCount = 0;

	//EnTT
	entt::registry Registry = entt::registry();

	// Control States
	HamiltonEngine::OpenGL::Camera Camera{
		HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION, HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT, HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP,
			0.0f, 0.0f, FieldOfView};

}
