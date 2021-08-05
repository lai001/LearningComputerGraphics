#pragma once
#include <vector>
#include "TextureDescription.h"
#include "BaseVertex.h"

class FStaticSubMesh
{

public:
	FStaticSubMesh(std::vector<FBaseVertex> Vertices, std::vector<unsigned int> Indices, std::vector<FTextureDescription*> Textures);
	~FStaticSubMesh();

public:
	std::vector<FBaseVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<FTextureDescription*> Textures;

};
