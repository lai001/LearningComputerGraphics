#include "SkeletionSubMesh.h"

FSkeletionSubMesh::FSkeletionSubMesh(std::vector<FVertex> Vertices, std::vector<unsigned int> Indices, std::vector<FTextureDescription*> Textures)
	:Vertices(Vertices), Indices(Indices), Textures(Textures)
{
}

FSkeletionSubMesh::~FSkeletionSubMesh()
{
}
