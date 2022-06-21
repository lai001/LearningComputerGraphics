#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "Interface/GUISystem.hpp"
#include "GUI/GLFWInputSystem.hpp"

class GLFWGUISystem: public IGUISystem
{
public:
	struct FCreateInfo
	{
		unsigned int Width = 1280;
		unsigned int Height = 720;
		std::string WindowName = "";
	};

private:
	GLFWwindow* glfwWindow = nullptr;
	//D3D11Contex* d3dContext = nullptr;
	GLFWInputSystem* glfwInputSystem = nullptr;

	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwCursorPositionCallback(GLFWwindow * window, double xpos, double ypos);
	static void glfwScrollCallback(GLFWwindow * window, double xoffset, double yoffset);
	static void glfwWindowSizeCallback(GLFWwindow * window, int width, int height);

public:
	GLFWGUISystem(const FCreateInfo& CreateInfo);
	~GLFWGUISystem();
	virtual void Render(double RunningTime) override;

	GLFWInputSystem * GetGLFWInputSystem() const noexcept;

	void Close();

	bool ShouldClose();

	HWND GetWin32Window();

	GLFWwindow* GetGlfwWindow();

	double CurrentTime();
};