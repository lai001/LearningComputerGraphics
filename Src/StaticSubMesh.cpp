#include "StaticSubMesh.h"

FStaticSubMesh::FStaticSubMesh(std::vector<FBaseVertex> Vertices, std::vector<unsigned int> Indices, std::vector<FTextureDescription*> Textures)
	:Vertices(Vertices), Indices(Indices), Textures(Textures)
{
}

FStaticSubMesh::~FStaticSubMesh()
{
}
