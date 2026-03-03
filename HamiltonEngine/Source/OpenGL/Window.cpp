#include "PrecompiledHeader/Pch.h"
#include "Window.h"
#include <Configuration/Globals.h>
#include "Utils.h"

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

		HamiltonEngine::OpenGL::Camera& Camera = HamiltonEngine::Globals::ActiveCamera;
		float sensitivity = HamiltonEngine::ConfigurationVariable<float>("MouseSensitivity", 0.1f);

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
		float yoffset = lastY - ypos; // x-coords are reversed because reasons
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		Camera.yaw -= xoffset;
		Camera.pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (Camera.pitch > 89.0f)
			 Camera.pitch = 89.0f;
		if (Camera.pitch < -89.0f)
			Camera.pitch = -89.0f;

		if (Camera.yaw > 360.0)
		{
			Camera.yaw -= 360.0;
		}

		if (Camera.yaw < -360.0)
		{
			Camera.yaw += 360.0;
		}

		Eigen::Vector3f NewFront;
		// Leaving this in for reference, this is for a RIGHT handed co-ord system. 
		//NewFront << cos(DegToRad(camera.yaw)) * cos(DegToRad(camera.pitch)),
		//			sin(DegToRad(camera.pitch)),
		//			sin(DegToRad(camera.yaw))* cos(DegToRad(camera.pitch));

		// Left Hand Coord system
		NewFront = { cos(DegToRad(Camera.yaw)) * cos(DegToRad(Camera.pitch)),
					 sin(DegToRad(Camera.yaw)),
					 sin(DegToRad(Camera.pitch)) * cos(DegToRad(Camera.yaw))
		};

		NewFront.normalize();
		Camera.CameraFront = NewFront;
	}

	void ProcessMovement(GLFWwindow* window, float DeltaTime)
	{

		HamiltonEngine::OpenGL::Camera& camera = HamiltonEngine::Globals::ActiveCamera;
		const float CameraSpeed = HamiltonEngine::ConfigurationVariable<float>("MovementSpeed", 2.5) * DeltaTime;

		// Release the Camera
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			{
				int WindowHeight, WindowWidth;
				glfwGetWindowSize(window, &WindowHeight, &WindowWidth);
				glfwSetCursorPos(window, WindowHeight / 2, WindowWidth / 2);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}

		// Reset to the default Camera Position
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			HamiltonEngine::Globals::ActiveCamera.CameraPosition = HamiltonEngine::OpenGL::DEFAULT_CAMERA_POSITION;
			HamiltonEngine::Globals::ActiveCamera.CameraFront = HamiltonEngine::OpenGL::DEFAULT_CAMERA_FRONT;
			HamiltonEngine::Globals::ActiveCamera.CameraUp = HamiltonEngine::OpenGL::DEFAULT_CAMERA_UP;
			HamiltonEngine::Globals::ActiveCamera.yaw = HamiltonEngine::OpenGL::DEFAULT_CAMERA_YAW;
			HamiltonEngine::Globals::ActiveCamera.pitch = HamiltonEngine::OpenGL::DEFAULT_CAMERA_PITCH;
			PrintCameraDetails(camera);
		}


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.CameraPosition += CameraSpeed * camera.CameraFront;
			PrintCameraDetails(camera);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.CameraPosition -= CameraSpeed * camera.CameraFront;
			PrintCameraDetails(camera);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.CameraPosition -= camera.CameraFront.cross(camera.CameraUp).normalized() * CameraSpeed;
			PrintCameraDetails(camera);
			
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.CameraPosition += camera.CameraFront.cross(camera.CameraUp).normalized() * CameraSpeed;
			PrintCameraDetails(camera);
		}

		//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && CameraMoved)
		//{
		//	RenderDefaultCubes = !RenderDefaultCubes;
		//}

		
	}

}

