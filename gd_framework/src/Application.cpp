#include "Application.h"
#include <GLFW/glfw3.h>

void Application::Run()
{
	glfwInit();
	StartUp();
	MainLoop();
	Cleanup();
	glfwTerminate();
}
