#include "PhongShadingPSHConstants.hpp"

FPhongShadingPSHConstants FPhongShadingPSHConstantsDefault()
{
	FPhongShadingPSHConstants Constants;
	Constants.SpotLight = FSpotLightDefault();
	Constants.PointLight = FPointLightDefault();
	Constants.DirectionalLight = FDirectionalLightDefault();
	Constants.ViewPosition = glm::vec3();
	Constants.Shininess = 32.0f;
	Constants.isUnlit = 0;
	return Constants;
}

FSpotLight FSpotLightDefault()
{
	FSpotLight Light;
	Light.Position = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Direction = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.CutOff = glm::cos(glm::radians(10.0f));
	Light.OuterCutOff = glm::cos(glm::radians(12.5f));
	Light.Constant = 1.0f;
	Light.Linear = 0.027f;
	Light.Quadratic = 0.027f;
	Light.isEnable = 1;
	return Light;
}

FPointLight FPointLightDefault()
{
	FPointLight Light;
	Light.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Light.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	Light.Constant = 1.0f;
	Light.Linear = 0.22f;
	Light.Quadratic = 0.2f;
	Light.isEnable = 1;
	return Light;
}

FDirectionalLight FDirectionalLightDefault()
{
	FDirectionalLight Light;
	Light.Direction = float3(-0.2f, -1.0f, -0.3f);
	Light.Ambient = float3(0.2f, 0.2f, 0.2f);
	Light.Diffuse = float3(0.2f, 0.2f, 0.2f);
	Light.Specular = float3(0.2f, 0.2f, 0.2f);
	Light.isEnable = 1;
	return Light;
}