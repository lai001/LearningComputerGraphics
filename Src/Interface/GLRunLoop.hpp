#pragma once
class IRenderRunLoop
{
public:
	virtual void Render(double RunningTime) = 0;
};