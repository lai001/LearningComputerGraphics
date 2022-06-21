#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct FDirectionLightShadowMapInfo
{
	float Left = -4.0f; 
	float Right = 4.0f;
	float Bottom = -4.0f;
	float Top = 4.0f;
	float NearZ = 1.0f;
	float FarZ = 10.0f;

	glm::vec3 Eye = glm::vec3(0.0f, 4.0f, -6.5f);
	glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 GetProjetionMat()
	{
		return glm::ortho(Left, Right, Bottom, Top, NearZ, FarZ);
	}

	glm::mat4 GetViewMat()
	{
		return glm::lookAt(Eye, Center, Up);
	}
};