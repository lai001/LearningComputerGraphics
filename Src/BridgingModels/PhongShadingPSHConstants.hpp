#pragma once

#include "Util/Types.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct FDirectionalLight
{
	float3 Direction;
	float N1;
	float3 Ambient;
	float N2;
	float3 Diffuse;
	float N3;
	float3 Specular;
	float N4;
	float3 N5;
	int isEnable;
};

struct FPointLight
{
	float3 Position;
	float N1;
	float3 Ambient;
	float N2;
	float3 Diffuse;
	float N3;
	float3 Specular;
	float N4;
	float Constant;
	float Linear;
	float Quadratic;
	int isEnable;
};

struct FSpotLight
{
	float3 Position;
	float N1;
	float3 Direction;
	float N2;
	float3 Ambient;
	float N3;
	float3 Diffuse;
	float N4;
	float3 Specular;
	float N5;
	float CutOff;
	float OuterCutOff;
	float Constant;
	float Linear;
	float Quadratic;
	int isEnable;
	float N6;
	float N7;
};

struct FPhongShadingPSHConstants
{
	FSpotLight SpotLight;
	FPointLight PointLight;
	FDirectionalLight DirectionalLight;
	float3 ViewPosition;
	float Shininess;
	float3 N1;
	int isUnlit;
};

FDirectionalLight FDirectionalLightDefault();
FPointLight FPointLightDefault();
FSpotLight FSpotLightDefault();
FPhongShadingPSHConstants FPhongShadingPSHConstantsDefault();