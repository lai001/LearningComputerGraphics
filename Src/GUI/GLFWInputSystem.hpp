#pragma once
#include <vector>
#include <functional>
#include <GLFW/glfw3.h>
#include "GUI/Keyboard.hpp"
#include "Interface/Input.hpp"

class GLFWInputSystem
{
public:
	GLFWInputSystem(GLFWwindow* glfwWindow);
	~GLFWInputSystem();
	void AddInput(IInput* input);
	void AddInputs(std::vector<IInput*> inputs);
	void Notify(int key, int scancode, int action, int mods);
	void Notify();
	void NotifyMousePositionChange(double xpos, double ypos);
	void NotifyMouseScroll(double xoffset, double yoffset);
	void SetCursorMode(const Keyboard::CursorMode CursorMode);
	Keyboard::CursorMode GetCursorMode() const;

private:
	std::vector<IInput*> Inputs;

	GLFWwindow* glfwWindow = nullptr;
};