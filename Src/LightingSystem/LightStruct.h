#pragma once
#include "ThirdParty/glm.h"

struct FDirectionalLight
{
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	bool bIsDirectionalLightEnable = true;
};

struct FPointLight
{
	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

	bool bIsPointLightEnable = true;
};

struct FSpotLight
{
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;
	float CutOff;
	float OuterCutOff;

	bool bIsSpotLightEnable = true;
};

struct FShadow
{
	glm::vec4 LightProjectionView;
	float ProjectionNear;
	float ProjectionFar;

	glm::vec3 LightViewEye;
	glm::vec3 LightViewCenter;
	glm::vec3 LightViewUp;

	bool bIsShadowEnable = false;

	glm::mat4 GetLightProjectionMat() const
	{
		return glm::ortho(LightProjectionView.x,
			LightProjectionView.y,
			LightProjectionView.z,
			LightProjectionView.w,
			ProjectionNear,
			ProjectionFar);
	}

	glm::mat4 GetLightViewMat() const
	{
		return glm::lookAt(LightViewEye,
			LightViewCenter,
			LightViewUp);
	}

	glm::mat4 GetLightSpaceMat() const
	{
		return GetLightProjectionMat() * GetLightViewMat();
	}
};