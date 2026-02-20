#pragma once
#include "OpenGL.h"

namespace HamiltonEngine::OpenGL 
{
	// TODO: Probably make some sort of window manager class, or will we ever only have 1 window?
	GLFWwindow* createWindow(int width, int height, const char* title);
	void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);
	void processInput(GLFWwindow* window);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
}