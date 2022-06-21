#pragma once
#include <vector>
#include <Foundation/Foundation.hpp>
#include "DiligentRenderer.hpp"
#include "SkeletonMeshDrawable.hpp"
#include "BridgingModels/SkeletonMeshPhongShadingVSHConstants.hpp"
#include "Object/Camera.hpp"
#include "PhongShadingPSHConstantsResource.hpp"

class FSkeletonMeshPipeline : public ks::noncopyable
{
private:
	FDiligentRenderer* Renderer = nullptr;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PSO;

	Diligent::RefCntAutoPtr<Diligent::IBuffer>                VSConstants;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> SRB;

	void CreateResources(FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource);

public:
	FSkeletonMeshPipeline(FDiligentRenderer* Renderer, 
		FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource);
	~FSkeletonMeshPipeline();

	void Render(double RunningTime,
		FCamera* Camera,
		FSkeletonMeshDrawable* SkeletonMeshDrawable);

	FSkeletonMeshPhongShadingVSHConstants SkeletonMeshPhongShadingVSHConstants;
};
