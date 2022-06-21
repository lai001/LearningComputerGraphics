#pragma once
#include <vector>
#include <unordered_map>
#include <assimp/material.h>
#include "Object/StaticMesh.hpp"
#include "DiligentRenderer.hpp"
#include "SubStaticMeshDrawableData.hpp"

class FStaticMeshDrawable
{
private:
	const FStaticMesh* StaticMesh = nullptr;
	std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas;
	void CreateResource(FDiligentRenderer * Renderer);

public:
	FStaticMeshDrawable(FDiligentRenderer * renderer, const FStaticMesh* StaticMesh);
	FStaticMeshDrawable(std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas);
	~FStaticMeshDrawable();

	std::vector<FSubStaticMeshDrawableData> GetSubStaticMeshDrawableDatas() const;

	Diligent::DrawIndexedAttribs GetDrawAttrs(const FSubStaticMeshDrawableData& SubStaticMeshDrawableData) const noexcept;

	const FStaticMesh* GetStaticMesh() const noexcept;
};
