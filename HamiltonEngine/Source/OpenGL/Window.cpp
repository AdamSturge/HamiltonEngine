#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include "Window.h"
#include <iostream>


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

}

