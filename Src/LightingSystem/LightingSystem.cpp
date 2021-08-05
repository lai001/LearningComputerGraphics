#include "LightingSystem.h"
#include "ThirdParty/glm.h"

FLightingSystem::FLightingSystem()
{
	DirLight = {
	glm::vec3(-0.2f, -1.0f, -0.3f),
	glm::vec3(0.2f, 0.2f, 0.2f),
	glm::vec3(0.2f, 0.2f, 0.2f),
	glm::vec3(0.2f, 0.2f, 0.2f)
	};

	PointLight = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.22f,
		0.20f
	};

	SpotLight = {
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.027f,
		0.027f,
		glm::cos(glm::radians(10.0f)),
		glm::cos(glm::radians(12.5f))
	};
}

FLightingSystem::~FLightingSystem()
{
}