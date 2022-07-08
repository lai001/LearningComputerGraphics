#pragma once
#include <glm/glm.hpp>

class FBaseVertex
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	FBaseVertex()
	{
	}

	FBaseVertex(glm::vec3 Position)
		:Position(Position)
	{
	}

	FBaseVertex(glm::vec3 Position, glm::vec2 TexCoords, glm::vec3 Normal)
		:Position(Position), TexCoords(TexCoords), Normal(Normal)
	{
	}
};