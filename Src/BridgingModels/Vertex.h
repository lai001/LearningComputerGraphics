#pragma once
#include "ThirdParty/glm.h"

struct FVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	int Ids[4];
	float Weights[4];

	FVertex()
	{
		memset(Ids, 0, sizeof(Ids));
		memset(Weights, 0, sizeof(Weights));
	};

	void AddBoneData(int BoneID, float Weight)
	{
		for (int i = 0; i < (sizeof(Ids) / sizeof(Ids[0])); i++) {
			if (Weights[i] == 0.0) {
				Ids[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
	}
};