#pragma once
#include <vector>
#include <unordered_map>
#include <assimp/material.h>
#include "DiligentRenderer.hpp"

class FSubStaticMeshDrawableData
{
public:
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer;
	std::unordered_map<aiTextureType, Diligent::RefCntAutoPtr<Diligent::ITexture>> TextureDic;
	unsigned int IndexCount = 0;
	Diligent::RefCntAutoPtr<Diligent::ITextureView> GetTextureView(const aiTextureType TextureType);
};