#pragma once
#include "ThirdParty/glm.h"
#include "LightStruct.h"

class FLightingSystem
{

public:
	FLightingSystem();
	~FLightingSystem();

	FDirectionalLight DirLight;
	FPointLight PointLight;
	FSpotLight SpotLight;
	
	glm::vec3 ViewPosition;

	FShadow Shadow;

private:


};

