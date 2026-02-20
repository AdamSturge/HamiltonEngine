#include "PrecompiledHeader/Pch.h"

#include "Globals.h"


namespace HamiltonEngine::Globals 
{
	ConfigurationVariable<float> PhysicsTickLength = ConfigurationVariable<float>("PhysicsTickLength", 1.0f / 60.0f);
	ConfigurationVariable<bool> PhysicsSimEnabled = ConfigurationVariable<bool>("PhysicsSimEnabled", true);
	ConfigurationVariable<float> Epsilon = ConfigurationVariable<float>("Epsilon", 1e-4f);


	// Rendering + OpenGL stuff
	ConfigurationVariable<std::vector<float>> BackgroundColorRGB("BackgroundColorRGB", { 0.2f, 0.3f, 0.3f });
	ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
	ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
	ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");
	ConfigurationVariable<float> FieldOfView("FieldOfView", 30.0);
	GLuint FrameCount = 0;

	// Control States
	bool CaptureMouseMovement = false;
	Eigen::Vector3f DefaultCameraPosition(0.0f, 0.0f, 3.0f);
	Eigen::Vector3f DefaultCameraFront(0.0f, 0.0f, -1.0f);
	Eigen::Vector3f DefaultCameraUp(0.0f, 1.0f, 0.0f);

	HamiltonEngine::OpenGL::Camera camera{
		DefaultCameraPosition, DefaultCameraFront, DefaultCameraUp,
			-90, 0.0f, FieldOfView};
}
