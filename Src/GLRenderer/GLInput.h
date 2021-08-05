#pragma once
#include "ThirdParty/opengl.h"

class IGLInput
{
public:
	virtual void ProcessInput(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods) = 0;
	virtual void MouseCallback(GLFWwindow * Window, double Xpos, double Ypos) = 0;
	virtual void ContiguousInput(GLFWwindow * Window, int Key) = 0;
	virtual void ScrollCallback(GLFWwindow* Window, double Xoffset, double Yoffset) = 0;
};
