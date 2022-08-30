#pragma once
#include <vector>
#include <unordered_map>
#include <assimp/material.h>
#include "Object/StaticMesh.hpp"
#include "DiligentRenderer.hpp"
#include "SubStaticMeshDrawableData.hpp"
#include "IBLBake.hpp"

class FPBRStaticMeshDrawable
{
private:
	const FStaticMesh* StaticMesh = nullptr;
	std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas;
	void CreateResource(FDiligentRenderer * Renderer);
	const FIBLBake* IBLBake = nullptr;

public:
	FPBRStaticMeshDrawable(FDiligentRenderer * Renderer, const FStaticMesh* StaticMesh);
	FPBRStaticMeshDrawable(std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas);
	~FPBRStaticMeshDrawable();

	std::vector<FSubStaticMeshDrawableData> GetSubStaticMeshDrawableDatas() const;

	Diligent::DrawIndexedAttribs GetDrawAttrs(const FSubStaticMeshDrawableData& SubStaticMeshDrawableData) const noexcept;

	const FStaticMesh* GetStaticMesh() const noexcept;

	void SetIBLBake(const FIBLBake* IBLBake) noexcept;

	const FIBLBake* GetIBLBake() noexcept;
};
