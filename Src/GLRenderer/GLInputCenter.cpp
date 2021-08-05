#include "GLInputCenter.h"

std::unordered_map<intptr_t, FGLInputCenter*> FGLInputCenter::InputCenterMap;

FGLInputCenter::FGLInputCenter(GLFWwindow * Window)
	:Window(Window)
{
	glfwSetScrollCallback(Window, FGLInputCenter::ScrollCallback);
	glfwSetCursorPosCallback(Window, FGLInputCenter::MouseCallback);
	glfwSetKeyCallback(Window, FGLInputCenter::ProcessInput);
	SetCursorMode(ECursorMode::Disable);

	intptr_t Key = reinterpret_cast<intptr_t>(Window);
	FGLInputCenter::InputCenterMap[Key] = this;
}

FGLInputCenter::~FGLInputCenter()
{
}

ECursorMode FGLInputCenter::GetCursorMode()
{
	return CursorMode;
}

void FGLInputCenter::SetCursorMode(ECursorMode Mode)
{
	switch (Mode)
	{
	case ECursorMode::Normal:
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case ECursorMode::Hidden:
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case ECursorMode::Disable:
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		break;
	}
	CursorMode = Mode;
}

void FGLInputCenter::ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods)
{
	intptr_t MapKey = reinterpret_cast<intptr_t>(Window);
	FGLInputCenter* Center = InputCenterMap[MapKey];
	if (Center)
	{
		for (auto Iter = Center->Inputs.begin(); Iter != Center->Inputs.end(); Iter++) {
			IGLInput* Input = Iter->second;
			Input->ProcessInput(Window, Key, Scancode, Action, Mods);
		}
	}
}

void FGLInputCenter::MouseCallback(GLFWwindow * Window, double Xpos, double Ypos)
{
	intptr_t Key = reinterpret_cast<intptr_t>(Window);
	FGLInputCenter* Center = InputCenterMap[Key];
	if (Center)
	{
		for (auto Iter = Center->Inputs.begin(); Iter != Center->Inputs.end(); Iter++) {
			IGLInput* Input = Iter->second;
			Input->MouseCallback(Window, Xpos, Ypos);
		}
	}
}

void FGLInputCenter::ContiguousInput()
{
	for (auto Iter = Inputs.begin(); Iter != Inputs.end(); ++Iter) {
		IGLInput* Input = Iter->second;
		for (int Key = 0; Key <= GLFW_KEY_LAST; Key++)
		{
			if (PressKey(Key))
			{
				Input->ContiguousInput(Window, Key);
			}
		}
	}
}

void FGLInputCenter::ScrollCallback(GLFWwindow * Window, double Xoffset, double Yoffset)
{
	intptr_t Key = reinterpret_cast<intptr_t>(Window);
	FGLInputCenter* Center = InputCenterMap[Key];
	if (Center)
	{
		for (auto Iter = Center->Inputs.begin(); Iter != Center->Inputs.end(); Iter++) {
			IGLInput* Input = Iter->second;
			Input->ScrollCallback(Window, Xoffset, Yoffset);
		}
	}
}

void FGLInputCenter::AddInput(IGLInput * Input)
{
	if (Input)
	{
		intptr_t Key = reinterpret_cast<intptr_t>(Input);
		FGLInputCenter::Inputs[Key] = Input;
	}
}

void FGLInputCenter::RemoveInput(IGLInput * Input)
{
	if (Input)
	{
		intptr_t Key = reinterpret_cast<intptr_t>(Input);
		FGLInputCenter::Inputs.erase(Key);
	}
}

bool FGLInputCenter::PressKey(int Key)
{
	if (glfwGetKey(Window, Key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
