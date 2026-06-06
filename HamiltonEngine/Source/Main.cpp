#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"

#include "Logging/Logging.h"

#include "Physics/Systems/ParticleSystem.h"
#include "Physics/Systems/RigidBodySystem.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/SpringPotential.h"

#include <RenderingSystem/RenderingSystemBase.h>

#include <iostream>

#include <RenderingSystem/Utils.h>
#include "Configuration/Globals.h"
#include "Serialization/JsonSerialization.h"


int main(int argc, char** argv)
{
	std::srand(std::time(0));
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	const char* StartingLevel = HamiltonEngine::Globals::StartingLevel.Get().c_str();
	HamiltonEngine::Serialization::DeserializeEnttRegistryFromJson(HamiltonEngine::Globals::Registry, StartingLevel);
	
	HamiltonEngine::ConfigurationVariable<int> WindowHeight("WindowHeight", 800);
	HamiltonEngine::ConfigurationVariable<int> WindowWidth("WindowWidth", 600);
	HamiltonEngine::ConfigurationVariable<std::string> WindowName("WindowName", "MyWindow");

	HamiltonEngine::RenderingSystem::SetupRenderingSystem();
	//HamiltonEngine::Physics::CreateParticleEntities();
	//HamiltonEngine::Physics::CreateRigidBodyEntities();

	HamiltonEngine::Globals::FrameCount = 0;
	float CurTime = 0.0f;
	float OldTime = 0.0f;
	float DeltaTime = 0.0f;

	while (!glfwWindowShouldClose(HamiltonEngine::Globals::MainWindow)) {
		++HamiltonEngine::Globals::FrameCount;

		// How long as it been since the last frame?
		OldTime = CurTime;
		CurTime = static_cast<float>(glfwGetTime());
		DeltaTime = CurTime - OldTime;

		// input
		HamiltonEngine::RenderingSystem::processInput(HamiltonEngine::Globals::MainWindow);
		HamiltonEngine::RenderingSystem::ProcessMovement(HamiltonEngine::Globals::MainWindow, DeltaTime);

		// physics
		HamiltonEngine::Physics::ParticleSystem();
		HamiltonEngine::Physics::RigidBodySystem();

		// rendering
		HamiltonEngine::RenderingSystem::Tick();
	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();

	//constexpr bool OverwriteLevelFile = true;
	//HamiltonEngine::Serialization::SerializeEnttRegistryAsJson(HamiltonEngine::Globals::Registry, 
	//	StartingLevel, OverwriteLevelFile);
	return 0;
}