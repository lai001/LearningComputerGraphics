#pragma once
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

private:


};

