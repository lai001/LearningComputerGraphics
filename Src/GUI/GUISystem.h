#pragma once
#include "ThirdParty/opengl.h"
#include "ThirdParty/imgui.h"
#include "GUIInterface.h"
#include "GLRenderer/GLRenderer.h"

class FGUISystem: public IGLRunLoop
{
private:
	GLFWwindow* Window;
	void Init();
	void Destroy();

public:
	FGUISystem(GLFWwindow* Window);
	~FGUISystem();

	IGUIInterface* Interface;

	const GLFWwindow* GetWindow();
	void Update();

	virtual void Tick(GLFWwindow * Window, double RunningTime) override;
};

