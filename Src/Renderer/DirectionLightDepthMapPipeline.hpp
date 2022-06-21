#pragma once
#include "DiligentRenderer.hpp"
#include "BridgingModels/DirectionLightDepthMapVSHConstants.hpp"
#include "StaticMeshDrawable.hpp"
#include "SkeletonMeshDrawable.hpp"

class FDirectionLightDepthMapPipeline
{
private:
	unsigned int Width = 512;
	unsigned int Height = 512;

	Diligent::RefCntAutoPtr<Diligent::ITexture> Texture;
	Diligent::RefCntAutoPtr<Diligent::ITextureView> ShadowMapDSV;
	Diligent::RefCntAutoPtr<Diligent::ITextureView> ShadowMapSRV;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PSO;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VSConstants;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> ShadowSRB;

	FDiligentRenderer* Renderer = nullptr;

	void CreateResources();

public:
	FDirectionLightDepthMapPipeline(FDiligentRenderer* Renderer,
		const unsigned int Width,
		const unsigned int Height);
	~FDirectionLightDepthMapPipeline();


	FDirectionLightDepthMapVSHConstants DirectionLightDepthMapVSHConstants;

	void Render(
		double RunningTime,
		FStaticMeshDrawable* StaticMeshDrawable);

	void NewFrame();

	Diligent::RefCntAutoPtr<Diligent::ITextureView> GetRenderTargetView();
	Diligent::RefCntAutoPtr<Diligent::ITextureView> GetResourceView();
	Diligent::RefCntAutoPtr<Diligent::ITexture> GetTexture() noexcept;
};
