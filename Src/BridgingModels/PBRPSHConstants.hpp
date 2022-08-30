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
	int isDebugViewEnable;
	int isDebugRoughnessTexture;
	int isDebugMetallicTexture;
	int isDebugAlbedoTexture;
	int isDebugNormalTexture;
	int isDebugIBLView;
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
	Constants.isDebugViewEnable = 0;
	Constants.isDebugRoughnessTexture = 0;
	Constants.isDebugMetallicTexture = 0;
	Constants.isDebugAlbedoTexture = 0;
	Constants.isDebugNormalTexture = 0;
	Constants.isDebugIBLView = 0;
	return Constants;
}
