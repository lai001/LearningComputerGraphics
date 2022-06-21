#pragma once
#include <string>

class IObjectPropertyUIComponent
{
public:
	virtual std::string GetName() = 0;
	virtual void Draw() = 0;
};