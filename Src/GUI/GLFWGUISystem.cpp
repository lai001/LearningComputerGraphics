#include "GLFWGUISystem.hpp"
#include <assert.h>

void GLFWGUISystem::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	void* userData = glfwGetWindowUserPointer(window);
	GLFWGUISystem* GUISystem = reinterpret_cast<GLFWGUISystem*>(userData);
	GUISystem->glfwInputSystem->Notify(key, scancode, action, mods);
}

void GLFWGUISystem::glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	void* userData = glfwGetWindowUserPointer(window);
	GLFWGUISystem* GUISystem = reinterpret_cast<GLFWGUISystem*>(userData);
	GUISystem->glfwInputSystem->NotifyMousePositionChange(xpos, ypos);
}

void GLFWGUISystem::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	void* userData = glfwGetWindowUserPointer(window);
	GLFWGUISystem* GUISystem = reinterpret_cast<GLFWGUISystem*>(userData);
	GUISystem->glfwInputSystem->NotifyMouseScroll(xoffset, yoffset);
}

void GLFWGUISystem::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	void* userData = glfwGetWindowUserPointer(window);
	GLFWGUISystem* GUISystem = reinterpret_cast<GLFWGUISystem*>(userData);
	GUISystem->GUIRenderer->WindowSizeChange(width, height);
}

GLFWGUISystem::GLFWGUISystem(const FCreateInfo& CreateInfo)
{
	int status = glfwInit();
	assert(GLFW_TRUE == status);
	glfwWindow = glfwCreateWindow(CreateInfo.Width,
		CreateInfo.Height, 
		CreateInfo.WindowName.c_str(), 
		nullptr, 
		nullptr);
	assert(glfwWindow);
	glfwSetWindowUserPointer(glfwWindow, this);
	glfwInputSystem = new GLFWInputSystem(glfwWindow);
	glfwSetKeyCallback(glfwWindow, glfwKeyCallback);
	glfwSetWindowSizeCallback(glfwWindow, glfwWindowSizeCallback);
	glfwSetCursorPosCallback(glfwWindow, glfwCursorPositionCallback);
	glfwSetScrollCallback(glfwWindow, glfwScrollCallback);
}

GLFWGUISystem::~GLFWGUISystem()
{
	glfwTerminate();
}

void GLFWGUISystem::Render(double RunningTime)
{
	if (GUIRenderer)
	{
		GUIRenderer->Render(RunningTime);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		glfwInputSystem->Notify();
	}
}

GLFWInputSystem * GLFWGUISystem::GetGLFWInputSystem() const noexcept
{
	return glfwInputSystem;
}

void GLFWGUISystem::Close()
{
	glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}

bool GLFWGUISystem::ShouldClose()
{
	return (glfwWindowShouldClose(glfwWindow) == GLFW_TRUE);
}

HWND GLFWGUISystem::GetWin32Window()
{
	HWND hwnd = glfwGetWin32Window(glfwWindow);
	return hwnd;
}

GLFWwindow * GLFWGUISystem::GetGlfwWindow()
{
	return glfwWindow;
}

double GLFWGUISystem::CurrentTime()
{
	return glfwGetTime();
}

