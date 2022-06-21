#pragma once

#include "Interface/GLRunLoop.hpp"
#include "GUIRenderer.hpp"

class IGUISystem: public IRenderRunLoop
{
public:
	IGUIRenderer* GUIRenderer = nullptr;
};