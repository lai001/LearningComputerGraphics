#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util/Types.hpp"

struct FSkeletonMeshPhongShadingVSHConstants
{
	static constexpr unsigned int MAX_BONES = 255;

	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
	float4x4 Bones[MAX_BONES];

	FSkeletonMeshPhongShadingVSHConstants() :
		Model(glm::identity<glm::mat4>()), 
		View(glm::identity<glm::mat4>()), 
		Projection(glm::identity<glm::mat4>())
	{
		for (auto& Bone : Bones)
		{
			Bone = glm::identity<glm::mat4>();
		}
	}
};
