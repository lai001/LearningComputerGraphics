#pragma once
#include "ThirdParty/glm.h"
#include "LightStruct.h"

class FLightingSystem
{

public:
	FLightingSystem();
	~FLightingSystem();

	FDirLight DirLight;
	FPointLight PointLight;
	FSpotLight SpotLight;

	bool bIsSpotLightEnable;
	glm::vec3 ViewPosition;

private:


};

