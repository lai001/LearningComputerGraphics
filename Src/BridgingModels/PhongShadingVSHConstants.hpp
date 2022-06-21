#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util/Types.hpp"

struct FPhongShadingVSHConstants
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
	float4x4 LightSpaceMatrix;
};
