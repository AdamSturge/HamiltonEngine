#include "PrecompiledHeader/Pch.h"
#include "Window.h"
#include <Configuration/Globals.h>
#include "Utils.h"
#include "Window.h"

namespace
{
	bool firstMouse = true;
	float lastX;
	float lastY;
}

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

			glfwSetCursorPosCallback(window, HamiltonEngine::OpenGL::mouse_callback);
			glfwSetScrollCallback(window, HamiltonEngine::OpenGL::scroll_callback);

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
		HamiltonEngine::Globals::ActiveCamera.fov -= (float)yoffset;
		if (HamiltonEngine::Globals::ActiveCamera.fov < 1.0f)
			HamiltonEngine::Globals::ActiveCamera.fov = 1.0f;
		if (HamiltonEngine::Globals::ActiveCamera.fov > 45.0f)
			HamiltonEngine::Globals::ActiveCamera.fov = 45.0f;
	}

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			return;

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			int wid, hei;
			glfwGetWindowSize(window, &wid, &hei);
			lastX = wid / 2.0f;
			lastY = hei / 2.0f;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;

		HamiltonEngine::OpenGL::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;
		
		HamiltonEngine::OpenGL::ProcessMouseMovement(Camera, xoffset, yoffset);

		PrintCameraDetails(Camera);
	}

	void ProcessMovement(GLFWwindow* Window, float DeltaTime)
	{

		HamiltonEngine::OpenGL::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;
		const float CameraSpeed = HamiltonEngine::ConfigurationVariable<float>("MovementSpeed", 2.5) * DeltaTime;

		// Release the Camera
		if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (glfwGetInputMode(Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			{
				int WindowHeight, WindowWidth;
				glfwGetWindowSize(Window, &WindowHeight, &WindowWidth);
				glfwSetCursorPos(Window, WindowHeight / 2, WindowWidth / 2);
				glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
				glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}

		// Reset to the default Camera Position
		if (glfwGetKey(Window, GLFW_KEY_R) == GLFW_PRESS)
		{
			Camera.CameraPosition = HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION;
			Camera.CameraFront = HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT;
			Camera.CameraUp = HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP;
			Camera.yaw = HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW;
			Camera.pitch = HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH;
		}


		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			HamiltonEngine::OpenGL::ProcessKeyboardMovement(Window, Camera, FORWARD, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			HamiltonEngine::OpenGL::ProcessKeyboardMovement(Window, Camera, BACKWARD, DeltaTime);
		}

		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			HamiltonEngine::OpenGL::ProcessKeyboardMovement(Window, Camera, LEFT, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			HamiltonEngine::OpenGL::ProcessKeyboardMovement(Window, Camera, RIGHT, DeltaTime);
		}

		//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && CameraMoved)
		//{
		//	RenderDefaultCubes = !RenderDefaultCubes;
		//}
		
	}

}

