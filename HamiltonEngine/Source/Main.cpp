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
		// Clean up this
		HamiltonEngine::RenderingSystem::Tick();
		glClearColor(WindowBackgroundRed, WindowBackgroundGreen, WindowBackgroundBlue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 2);

		//simpleShader.setFloat("TIME", CurTime);
		//simpleShader.setFloat("mixRatio", sin(CurTime));
		simpleShader.use();
		
		GLint modelLoc = glGetUniformLocation(simpleShader.ID, "model");		
		GLint viewLoc = glGetUniformLocation(simpleShader.ID, "view");
		View = HamiltonEngine::OpenGL::LookAt(Camera.CameraPosition, Camera.CameraPosition + Camera.CameraFront, Camera.WorldUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, View.data());
		GLint projLoc = glGetUniformLocation(simpleShader.ID, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, Projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Model.data());

		// Will Render anything with a TransformComponent and a OpenGLBuffersComponent
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		HamiltonEngine::OpenGL::Render(modelLoc);

		//glBindVertexArray(HamiltonEngine::Globals::PrimativesBuffers["cube"].VAO);
		auto RigidBodyView = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::RigidBodyStateComponent>();

		for (auto [Entity, StateC] : RigidBodyView.each())
		{

			HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"], StateC.Transform, modelLoc);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, StateC.Transform.data());

			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

		}

		// Use a different texture for the sping ends
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 2);

		auto Springs = HamiltonEngine::Globals::Registry.view<HamiltonEngine::Physics::SpringPotentialComponent>();
		Eigen::Vector3f SpringModelScale = Eigen::Vector3f(5.0f, 5.0f, 5.0f);
		int a = 0;
		for (auto [Entity, SpringComp] : Springs.each())
		{
			auto Parent = HamiltonEngine::Globals::Registry.get<HamiltonEngine::Physics::RigidBodyStateComponent>(SpringComp.ParentEntity);

			Eigen::Affine3f SpringWorldTransform = Parent.Transform.translate(SpringComp.AnchorPointBody)
																	.scale(SpringModelScale);
			
			HamiltonEngine::OpenGL::RenderBuffer(HamiltonEngine::Globals::PrimativesBuffers["cube"], SpringWorldTransform, modelLoc);
		}

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	
	std::cout << "There were " << HamiltonEngine::Globals::FrameCount << " frames rendered." << std::endl;
	std::cout << "The average frame time " << glfwGetTime() / HamiltonEngine::Globals::FrameCount << std::endl;
	glfwTerminate();

	//constexpr bool OverwriteLevelFile = true;
	//HamiltonEngine::Serialization::SerializeEnttRegistryAsJson(HamiltonEngine::Globals::Registry, 
	//	StartingLevel, OverwriteLevelFile);
	return 0;
}