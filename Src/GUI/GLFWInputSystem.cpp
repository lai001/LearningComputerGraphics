#include "GLFWInputSystem.hpp"
#include "GUI/GLFWGUISystem.hpp"
#include <assert.h>

GLFWInputSystem::GLFWInputSystem(GLFWwindow* glfwWindow)
	:glfwWindow(glfwWindow)
{
	
}

GLFWInputSystem::~GLFWInputSystem()
{
}

void GLFWInputSystem::AddInput(IInput* input)
{
	Inputs.push_back(input);
}

void GLFWInputSystem::AddInputs(std::vector<IInput*> inputs)
{
	this->Inputs.insert(this->Inputs.end(), inputs.begin(), inputs.end());
}

void GLFWInputSystem::Notify(int key, int scancode, int action, int mods)
{
	for (auto& input : Inputs)
	{
		input->NotifyKeyInput(static_cast<Keyboard::KeyType>(key),
			static_cast<Keyboard::ActionType>(action),
			Keyboard::ModeFlag(mods));
	}
}

void GLFWInputSystem::Notify()
{
	for (auto& input : Inputs)
	{
		input->NotifyKeyActionChange([this](Keyboard::KeyType keyType)
		{
			int action = glfwGetKey(glfwWindow, static_cast<int>(keyType));
			Keyboard::ActionType actionType = static_cast<Keyboard::ActionType>(action);
			return actionType;
		});
	}
}

void GLFWInputSystem::NotifyMousePositionChange(double xpos, double ypos)
{
	for (auto& input : Inputs)
	{
		input->NotifyMousePositionChange(xpos, ypos);
	}
}

void GLFWInputSystem::NotifyMouseScroll(double xoffset, double yoffset)
{
	for (auto& input : Inputs)
	{
		input->NotifyMouseScroll(xoffset, yoffset);
	}
}

void GLFWInputSystem::SetCursorMode(const Keyboard::CursorMode CursorMode)
{
	if (CursorMode == Keyboard::CursorMode::Normal)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		assert(GetCursorMode() == Keyboard::CursorMode::Normal);
	}
	else if (CursorMode == Keyboard::CursorMode::Hidden)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		assert(GetCursorMode() == Keyboard::CursorMode::Hidden);
	}
	else if (CursorMode == Keyboard::CursorMode::Disabled)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		assert(GetCursorMode() == Keyboard::CursorMode::Disabled);
	}
	else
	{
		assert(false);
	}
}

Keyboard::CursorMode GLFWInputSystem::GetCursorMode() const
{
	int CursorMode = glfwGetInputMode(glfwWindow, GLFW_CURSOR);
	if (CursorMode == GLFW_CURSOR_NORMAL)
	{
		return Keyboard::CursorMode::Normal;
	}
	else if (CursorMode == GLFW_CURSOR_HIDDEN)
	{
		return Keyboard::CursorMode::Hidden;
	}
	else if (CursorMode == GLFW_CURSOR_DISABLED)
	{
		return Keyboard::CursorMode::Disabled;
	}
	else
	{
		assert(false);
	}
	return Keyboard::CursorMode::Normal;
}
