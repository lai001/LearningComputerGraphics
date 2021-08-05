#pragma once
#include <vector>
#include "TextureDescription.h"
#include "BridgingModels/BridgingModels.h"

class FSkeletionSubMesh
{
public:
	FSkeletionSubMesh(std::vector<FVertex> Vertices, std::vector<unsigned int> Indices, std::vector<FTextureDescription*> Textures);
	~FSkeletionSubMesh();

public:
	std::vector<FVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;

};
