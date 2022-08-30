#include "Math.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

/*
* http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
*/
float FMath::RadicalInverseVdC(unsigned int bits) noexcept
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

glm::vec2 FMath::Hammersley2D(unsigned int i, unsigned int N) noexcept
{
	return glm::vec2(float(i) / float(N), RadicalInverseVdC(i));
}

glm::vec3 FMath::HemisphereSampleUniform(float u, float v) noexcept
{
	float phi = v * 2.0 * M_PI;
	float cosTheta = 1.0 - u;
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	return glm::vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

glm::vec3 FMath::ImportanceSampleGGX(const glm::vec2& Xi, const float roughness) noexcept
{
	float a = roughness * roughness;
	float phi = 2.0 * M_PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	glm::vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	return H;
}

glm::vec2 FMath::SampleEquirectangularMap(const glm::vec3 SamplePicker) noexcept
{
	const float X = glm::clamp<float>((atan2(SamplePicker.z, SamplePicker.x) + M_PI) / (M_PI * 2.0f), 0.0f, 1.0f);
	const float Y = glm::clamp<float>(acos(SamplePicker.y) / M_PI, 0.0f, 1.0f);
	return glm::vec2(X, Y);
}

glm::vec3 FMath::ConvertCoordinateSystem(const glm::vec3 & V, const glm::vec3 & X, const glm::vec3 & Y, const glm::vec3 & Z) noexcept
{
	const glm::mat4 C = glm::mat4(glm::vec4(X, 0.0f), glm::vec4(Y, 0.0f), glm::vec4(Z, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	const glm::vec4 V1 = C * glm::vec4(V, 1.0f);
	return glm::vec3(V1);
}

float FMath::GeometrySchlickGGX(float NdotV, float Roughness) noexcept
{
	float a = Roughness;
	float k = (a * a) / 2.0f;
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom / denom;
}

float FMath::GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float Roughness) noexcept
{
	float NdotV = glm::max(glm::dot(N, V), 0.0f);
	float NdotL = glm::max(glm::dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, Roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, Roughness);
	return ggx1 * ggx2;
}
