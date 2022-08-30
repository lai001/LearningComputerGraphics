#pragma once

#include <glm/glm.hpp>

class FMath
{
public:
	static float RadicalInverseVdC(unsigned int bits) noexcept;
	static glm::vec2 Hammersley2D(unsigned int i, unsigned int N) noexcept;
	static glm::vec3 HemisphereSampleUniform(float u, float v) noexcept;
	static glm::vec3 ImportanceSampleGGX(const glm::vec2& Xi, const float roughness) noexcept;
	static glm::vec2 SampleEquirectangularMap(const glm::vec3 SamplePicker) noexcept;
	static glm::vec3 ConvertCoordinateSystem(const glm::vec3& V, const glm::vec3& X, const glm::vec3& Y, const glm::vec3& Z) noexcept;
	static float GeometrySchlickGGX(float NdotV, float Roughness) noexcept;
	static float GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float Roughness) noexcept;
};
