#pragma once
#include "Interface/GLRunLoop.hpp"

class IGUIRenderer : public IRenderRunLoop
{
public:
	virtual void WindowSizeChange(const int width, const int height) = 0;

};