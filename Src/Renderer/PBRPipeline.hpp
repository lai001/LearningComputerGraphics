#pragma once
#include <vector>
#include <Foundation/Foundation.hpp>
#include "DiligentRenderer.hpp"
#include "PBRStaticMeshDrawable.hpp"
#include "Object/Camera.hpp"
#include "BridgingModels/PBRVSHConstants.hpp"
#include "BridgingModels/PBRPSHConstants.hpp"

class FPBRPipeline : public ks::noncopyable
{
private:
	FDiligentRenderer* Renderer = nullptr;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PSO;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VSConstants;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> PSConstants;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> SRB;

	void CreateResources();

public:
	FPBRPipeline(FDiligentRenderer* Renderer);
	~FPBRPipeline();

	void Render(
		FCamera* Camera,
		FPBRStaticMeshDrawable* PBRStaticMeshDrawable);

	FPBRVSHConstants PBRVSHConstants;
	FPBRPSHConstants PBRPSHConstants;
};
