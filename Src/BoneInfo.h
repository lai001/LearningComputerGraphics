#pragma once
#include <string>
#include "ThirdParty/assimp.h"
#include "ThirdParty/glm.h"

struct FBoneInfo
{
	std::string BoneName = "";
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransformation;
	aiNodeAnim* NodeAnimation;

	FBoneInfo()
		:BoneOffset(glm::mat4(1.0)), FinalTransformation(glm::mat4(1.0)), NodeAnimation(nullptr)
	{
	}
};