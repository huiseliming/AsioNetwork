#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window()
{
}

Window::Window(const char * applicationName, int width, int height)
{
	Create(applicationName, width, width);
}

Window::~Window()
{
	Destroy();
}

void Window::Create(const char* applicationName, int width, int height)
{
	m_width = width;
	m_height = height;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_pGLFWWindow = glfwCreateWindow(m_width, m_height, applicationName, nullptr, nullptr);
	//glfwSetWindowUserPointer(m_pGLFWWindow, application_ptr__);
	//glfwSetFramebufferSizeCallback(m_pGLFWWindow, Window::FramebufferResizeCallback);
}

void Window::Destroy()
{
	if (m_pGLFWWindow != nullptr) {
		glfwDestroyWindow(m_pGLFWWindow);
		m_pGLFWWindow = nullptr;
	}
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	//auto application_ptr = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	//application_ptr->get_render_engine_ptr()->Resize();
}

GLFWwindow* Window::GetGLFWWindow() { return m_pGLFWWindow; }

bool Window::ShouldClose() 
{
	return glfwWindowShouldClose(m_pGLFWWindow);
}
