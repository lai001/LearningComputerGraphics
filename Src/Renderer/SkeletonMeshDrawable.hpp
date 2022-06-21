#pragma once
#include <vector>
#include <unordered_map>
#include <assimp/material.h>
#include "Object/SkeletonMesh.hpp"
#include "DiligentRenderer.hpp"
#include "SubStaticMeshDrawableData.hpp"

class FSkeletonMeshDrawable
{
private:
	const FSkeletonMesh* SkeletonMesh = nullptr;
	std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas;
	void CreateResource(FDiligentRenderer * renderer);

public:
	FSkeletonMeshDrawable(FDiligentRenderer * renderer, const FSkeletonMesh* SkeletonMesh);
	~FSkeletonMeshDrawable();

	std::vector<FSubStaticMeshDrawableData> GetSubStaticMeshDrawableDatas() const;

	Diligent::DrawIndexedAttribs GetDrawAttrs(const FSubStaticMeshDrawableData& SubStaticMeshDrawableData) const noexcept;

	const FSkeletonMesh* GetSkeletonMesh() const noexcept;
};
