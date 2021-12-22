#pragma once
#include "ThirdParty/opengl.h"

class IGLRunLoop
{
public:
	virtual void Tick(const GLFWwindow& Window, double RunningTime) = 0;

};
