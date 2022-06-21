#pragma once
#include <functional>
#include "GUIInterface.hpp"
#include "Interface/GLRunLoop.hpp"
#include "GUI/Keyboard.hpp"

class IInput
{
public:
	virtual void NotifyKeyInput(Keyboard::KeyType KeyType, Keyboard::ActionType ActionType, Keyboard::ModeFlag ModeFlag) = 0;
	virtual void NotifyKeyActionChange(std::function<Keyboard::ActionType(Keyboard::KeyType)> select) = 0;
	virtual void NotifyMousePositionChange(double Xpos, double Ypos) = 0;
	virtual void NotifyMouseScroll(double Xoffset, double Yoffset) = 0;
};