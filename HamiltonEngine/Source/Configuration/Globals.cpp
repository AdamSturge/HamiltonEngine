#include "PrecompiledHeader/Pch.h"
#include "Globals.h"


namespace HamiltonEngine::Globals 
{
	ConfigurationVariable<float> PhysicsTickLength = ConfigurationVariable<float>("PhysicsTickLength", 1.0f / 60.0f);
	ConfigurationVariable<bool> PhysicsSimEnabled = ConfigurationVariable<bool>("PhysicsSimEnabled", true);
	ConfigurationVariable<float> Epsilon = ConfigurationVariable<float>("Epsilon", 1e-4f);


	// Rendering + RenderingSystem stuff
	GLuint FrameCount = 0;
	std::map<std::string, HamiltonEngine::RenderingSystem::OpenGLBuffersComponent>  PrimativesBuffers = std::map<std::string,HamiltonEngine::RenderingSystem::OpenGLBuffersComponent>();
	//GLFWwindow* MainWindow;
	//EnTT
	entt::registry Registry = entt::registry();

	// Control States
	HamiltonEngine::RenderingSystem::Camera ActiveCamera;
}
