#pragma once
#include <vector>
#include <Foundation/Foundation.hpp>
#include "DiligentRenderer.hpp"
#include "StaticMeshDrawable.hpp"
#include "BridgingModels/PhongShadingVSHConstants.hpp"
#include "Object/Camera.hpp"
#include "PhongShadingPSHConstantsResource.hpp"

class FStaticMeshPipeline: public ks::noncopyable
{
public:
	struct FRenderInfo
	{
		double RunningTime = 0.0;
		FCamera* Camera = nullptr;
		FStaticMeshDrawable* StaticMeshDrawable = nullptr;
		Diligent::RefCntAutoPtr<Diligent::ITextureView> DirctionLightShadowMap;
	};

private:
	FDiligentRenderer* Renderer = nullptr;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PSO;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VSConstants;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> SRB;

	void CreateResources(FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource);

public:
	FStaticMeshPipeline(FDiligentRenderer* Renderer,
		FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource);
	~FStaticMeshPipeline();

	void Render(const FRenderInfo& RenderInfo);

	 FPhongShadingVSHConstants PhongShadingVSHConstants;
};
