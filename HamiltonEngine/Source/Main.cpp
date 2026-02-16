#include "PrecompiledHeader/Pch.h"

#include "Configuration/ConfigurationSystem.h"
#include "Configuration/ConfigurationVariable.h"
#include "Configuration/Globals.h"
#include "Physics/ParticleState.h"
#include "Physics/RigidBodyState.h"
#include "Physics/SymplecticEulerSystem.h"
#include "Physics/RigidBodyRattleSystem.h"

#include <OpenGl/OpenGL.h>
#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include "OpenGL/Texture.h"


namespace HamiltonEngine
{
	ConfigurationVariable<int> NumParticles("NumParticles", 10);
	ConfigurationVariable<int> NumRigidBodies("NumRigidBodies", 10);
	
	void CreatePhysicsEntities(entt::registry& Registry)
	{
		if (Globals::PhysicsSimEnabled) 
		{
			for (int EntityIndex = 0; EntityIndex < NumParticles; ++EntityIndex)
			{
				entt::entity Entity = Registry.create();
	
				Registry.emplace<Physics::PositionComponent>(Entity, Eigen::Vector3f::Zero());
				Registry.emplace<Physics::LinearMomentumComponent>(Entity, Eigen::Vector3f(1.0f,0.0f,0.0f));
				Registry.emplace<Physics::MassComponent>(Entity, 1.0f);
				Registry.emplace<Physics::GradParticlePotentialComponent>(Entity, Eigen::Vector3f::Zero());
			}

			for (int EntityIndex = 0; EntityIndex < NumRigidBodies; ++EntityIndex)
			{
				entt::entity Entity = Registry.create();

				Eigen::Affine3f A = Eigen::Affine3f::Identity();
				A.rotate(Eigen::AngleAxisf(5.0f, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
				Registry.emplace<Physics::TransformComponent>(Entity, A);
				Registry.emplace<Physics::AngularMomentumComponent>(Entity, Eigen::Matrix3f::Zero());
				Registry.emplace<Physics::MassTensorComponent>(Entity, Eigen::Matrix3f::Identity());
				Registry.emplace<Physics::GradRigidBodyPotentialComponent>(Entity, Eigen::Matrix3f::Random());
			}
		}	
	}

	//TODO move this to physics folder?
	void StepPhysics(entt::registry& Registry)
	{
		if (!Globals::PhysicsSimEnabled)
		{
			return;
		}
		
		//Particle Sim
		auto SymplecticEulerPhysicsSimView = Registry.view<
			HamiltonEngine::Physics::PositionComponent,
			HamiltonEngine::Physics::LinearMomentumComponent,
			HamiltonEngine::Physics::GradParticlePotentialComponent>();

		for (auto [Entity, PositionC, LinMomC, GradPotentialC] : SymplecticEulerPhysicsSimView.each())
		{
			HamiltonEngine::Physics::SymplecticEulerSystem(PositionC, LinMomC, GradPotentialC);
		}
		
		// Rigid Body Sim
		auto RigidBodyPhysicsSimView = Registry.view<
			HamiltonEngine::Physics::TransformComponent,
			HamiltonEngine::Physics::AngularMomentumComponent,
			HamiltonEngine::Physics::MassTensorComponent,
			HamiltonEngine::Physics::GradRigidBodyPotentialComponent>();

		for (auto [Entity, TransformC, AngularMomC, MassTensorC, GradPotentialC] : RigidBodyPhysicsSimView.each())
		{
			HamiltonEngine::Physics::RigidBodyRattleSystem(TransformC, AngularMomC, MassTensorC, GradPotentialC);
		}
	}
}

int main(int argc, char** argv)
{
	HamiltonEngine::ConfigurationSystem::Initialize("config.json", "user_config.json");

	entt::registry Registry;
	HamiltonEngine::CreatePhysicsEntities(Registry);

	glfwInit(); // Initialize OpenGL
	GLFWwindow* window = HamiltonEngine::OpenGL::createWindow(800, 600, "MyWindow");

	if (window == NULL)
	{
		return -1;
	}

	float red = 0.2f;
	float green = 0.3f;
	float blue = 0.3f;
	float curTime = 0.0f;

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3,  // second triangle

	};


	unsigned int VAO, VBO, EBO;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	HamiltonEngine::OpenGL::Texture texture1c = HamiltonEngine::OpenGL::Texture::Texture("container.jpg", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	texture1c.setActive();
	texture1c.setTextureOption(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	texture1c.setTextureOption(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	texture1c.setTextureOption(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture1c.setTextureOption(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	HamiltonEngine::OpenGL::Texture texture2c = HamiltonEngine::OpenGL::Texture::Texture("awesomeface.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	texture2c.setActive();
	texture2c.setTextureOption(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture2c.setTextureOption(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture2c.setTextureOption(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture2c.setTextureOption(GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	HamiltonEngine::OpenGL::Shader simpleShader = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs", 
																						 "Source\\Shaders\\fragmentShader.fs");

	HamiltonEngine::OpenGL::Shader simpleShader2 = HamiltonEngine::OpenGL::Shader::Shader("Source\\Shaders\\vertexShader.vs",
																						  "Source\\Shaders\\fragmentShader.fs");
	simpleShader.use();
	simpleShader.setInt("texture1", 0);
	simpleShader.setInt("texture2", 1);

	// glm::mat4 trans = glm::mat4(1.0f);
	Eigen::Affine3f amat;

	while (!glfwWindowShouldClose(window)) {

		// input
		HamiltonEngine::OpenGL::processInput(window);

		HamiltonEngine::StepPhysics(Registry);

		// rendering
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1c.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2c.ID);


		curTime = static_cast<float>(glfwGetTime());
		simpleShader.setFloat("TIME", curTime);
		simpleShader.setFloat("mixRatio", sin(curTime));

		/* This is using glm 
		trans = glm::mat4(1.0f); 
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, curTime, glm::vec3(0.0f, 0.0f, 1.0f));
		*/

		// This is using Eigen
		amat = Eigen::Affine3f::Identity();
		amat.translate(Eigen::Vector3f(0.5f, -0.5f, 0.0f));
		amat.rotate(Eigen::AngleAxisf(curTime, Eigen::Vector3f::UnitZ()));

		unsigned int transformLoc = glGetUniformLocation(simpleShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
			//glm::value_ptr(trans)
			amat.data()
			);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// This is using Eigen
		amat = Eigen::Affine3f::Identity();
		amat.translate(Eigen::Vector3f(-0.5, 0.5f, 0.0f));
		amat.scale(Eigen::Vector3f(abs(sin(curTime)), abs(sin(curTime)), 0.0f));

		/* This is using glm
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(abs(sin(curTime)), abs(sin(curTime)), 0.0f));
		*/

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
			//glm::value_ptr(trans)
			amat.data()
		);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}