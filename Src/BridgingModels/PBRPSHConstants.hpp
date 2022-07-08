#pragma once

#include "Util/Types.hpp"
#include "PhongShadingPSHConstants.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct FPBRPSHConstants
{
	FSpotLight SpotLight;
	FPointLight PointLight;
	FDirectionalLight DirectionalLight;
	float3 ViewPosition;
	float Shininess;
	float3 N1;
	int isUnlit;
	float RoughnessFactor;
	float MetalnessFactor;
};

inline FPBRPSHConstants FPBRPSHConstantsDefault()
{
	FPBRPSHConstants Constants;
	Constants.SpotLight = FSpotLightDefault();
	Constants.PointLight = FPointLightDefault();
	Constants.DirectionalLight = FDirectionalLightDefault();
	Constants.ViewPosition = glm::vec3();
	Constants.Shininess = 32.0f;
	Constants.isUnlit = 0;
	Constants.RoughnessFactor = 1.0f;
	Constants.MetalnessFactor = 1.0f;
	return Constants;
}
