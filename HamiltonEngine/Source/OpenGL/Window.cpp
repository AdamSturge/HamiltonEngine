#include "PrecompiledHeader/Pch.h"
#include "Window.h"
#include <Configuration/Globals.h>
#include "Utils.h"

namespace HamiltonEngine::OpenGL 
{
	GLFWwindow* createWindow(int width, int height, const char* title)
	{
		if (glfwInit() == GLFW_TRUE)  // Returns true immediately if initalized. 
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

			if (window == NULL)
			{
				std::cout << "Failed to create GLFW Window" << std::endl;
				glfwTerminate();
				return NULL;
			}

			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initalize GLAD" << std::endl;
				return NULL;
			}

			glViewport(0, 0, width, height);

			// Register callback functions AFTER we create the window but BEFORE render loop is started
			glfwSetFramebufferSizeCallback(window, HamiltonEngine::OpenGL::framebuffer_size_callback);

			return window;
		}
		else
		{
			return NULL;
		}
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int heigh)
	{
		glViewport(0, 0, width, heigh);
	}

	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		HamiltonEngine::Globals::camera.fov -= (float)yoffset;
		if (HamiltonEngine::Globals::camera.fov < 1.0f)
			HamiltonEngine::Globals::camera.fov = 1.0f;
		if (HamiltonEngine::Globals::camera.fov > 45.0f)
			HamiltonEngine::Globals::camera.fov = 45.0f;
	}

	bool firstMouse = true;
	float lastX = HamiltonEngine::Globals::WindowWidth / 2.0f;
	float lastY = HamiltonEngine::Globals::WindowHeight / 2.0f;

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{

		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			return;

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // x-coords are reversed because reasons
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		HamiltonEngine::Globals::camera.yaw += xoffset;
		HamiltonEngine::Globals::camera.pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (HamiltonEngine::Globals::camera.pitch > 89.0f)
			HamiltonEngine::Globals::camera.pitch = 89.0f;
		if (HamiltonEngine::Globals::camera.pitch < -89.0f)
			HamiltonEngine::Globals::camera.pitch = -89.0f;

		Eigen::Vector3f NewFront;
		NewFront << cos(DegToRad(HamiltonEngine::Globals::camera.yaw)) * cos(DegToRad(HamiltonEngine::Globals::camera.pitch)),
			sin(DegToRad(HamiltonEngine::Globals::camera.pitch)),
			sin(DegToRad(HamiltonEngine::Globals::camera.yaw))* cos(DegToRad(HamiltonEngine::Globals::camera.pitch));
		NewFront.normalize();
		HamiltonEngine::Globals::camera.CameraFront = NewFront;

	}

}

