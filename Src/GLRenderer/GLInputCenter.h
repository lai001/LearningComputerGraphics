#pragma once
#include <vector>
#include <unordered_map>

#include "ThirdParty/opengl.h"
#include "GLInput.h"

enum class ECursorMode
{
	Normal = 0,
	Hidden = 1,
	Disable = 2
};

class FGLInputCenter
{
private:
	GLFWwindow* Window;
	ECursorMode CursorMode;

public:
	FGLInputCenter(GLFWwindow * Window);
	~FGLInputCenter();
	std::unordered_map<intptr_t, IGLInput*> Inputs;
	static std::unordered_map<intptr_t, FGLInputCenter*> InputCenterMap;

	ECursorMode GetCursorMode();
	void SetCursorMode(ECursorMode Mode);

	void ContiguousInput();

	static void ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods);
	static void MouseCallback(GLFWwindow * Window, double Xpos, double Ypos);
	static void ScrollCallback(GLFWwindow* Window, double Xoffset, double Yoffset);

	void AddInput(IGLInput* Input);
	void RemoveInput(IGLInput* Input);

	bool PressKey(int Key);
};
