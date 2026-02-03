#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);
void processInput(GLFWwindow* window);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// Register callback functions AFTER we create the window but BEFORE render loop is started
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float color_step = 0.01f;
	float red = 0.2f;
	float green = 0.3f;
	float blue = 0.3f;

	using namespace std::chrono_literals;

	while (!glfwWindowShouldClose(window)) {

		// input
		processInput(window);

		// rendering

		red += color_step;
		green += color_step;
		blue += color_step;

		if (red > 1.0) {
			red = red - 1.0f;
		}
		if (green > 1.0) {
			green = green - 1.0f;
		}
		if (blue > 1.0) {
			blue = blue - 1.0f;
		}

		std::this_thread::sleep_for(20ms);

		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// swap buffers and call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
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
