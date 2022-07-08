#pragma once
#include <glm/glm.hpp>

struct FPBRStaticMeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangents;
	glm::vec3 Bitangents;
	glm::vec2 TexCoords;
};