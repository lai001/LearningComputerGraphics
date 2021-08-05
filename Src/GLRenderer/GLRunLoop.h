#pragma once
#include "ThirdParty/opengl.h"

class IGLRunLoop
{
public:
	virtual void Tick(GLFWwindow* Window, double RunningTime) = 0;

};
