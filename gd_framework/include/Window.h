#pragma once

struct GLFWwindow;

class Window
{
public:
	Window();
	explicit Window(const char* applicationName, int width = 800, int height = 600);
	virtual ~Window();

	void Create(const char* applicationName = "Application", int width = 800, int height = 600);
	void Destroy();

	void PollEvents();
	bool ShouldClose();


	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* GetGLFWWindow();

private:
	GLFWwindow* m_pGLFWWindow;
	int m_width;
	int m_height;
};


