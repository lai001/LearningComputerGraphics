#pragma once
#include "GUIInterface.h"
#include "GUISystem.h"

class FNutGUI: public IGUIInterface
{
public:
	FNutGUI();
	~FNutGUI();

	virtual void Render(float RunningTime) override;

public:

	glm::vec3 PointLightPosition = glm::vec3(0.0f);
	float CurrentFOV = 0.0f;
	float CurrentCameraSpeed = 0.0f;
	int PostProcessing = 0;
	float ModelAnimationSpeed = 1.0f;
	bool BindingPostEnable = false;
	bool bIsFrameBufferEnable = false;
	bool bIsShowSkybox = true;

};

