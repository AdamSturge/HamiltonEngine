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
	extern ConfigurationVariable<std::vector<float>> BackgroundColorRGB;
	extern ConfigurationVariable<int> WindowHeight;
	extern ConfigurationVariable<int> WindowWidth;
	extern ConfigurationVariable<std::string> WindowName;
	extern ConfigurationVariable<float> FieldOfView;
	extern GLuint FrameCount;
	
	// Default Camera Location/Look AT
	extern Eigen::Vector3f DefaultCameraPosition;
	extern Eigen::Vector3f DefaultCameraFront;
	extern Eigen::Vector3f DefaultCameraUp;
	extern HamiltonEngine::OpenGL::Camera camera;

	
}