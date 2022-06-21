#pragma once
#include "DiligentRenderer.hpp"
#include "SkyBoxDrawable.hpp"
#include "BridgingModels/SkyBoxVSHConstants.hpp"
#include "Object/Camera.hpp"

class FSkyBoxPipeline
{
private:
	FDiligentRenderer* Renderer = nullptr;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState>			PSO;
	Diligent::RefCntAutoPtr<Diligent::IBuffer>					VSConstants;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding>	SRB;
	void CreateResources();

public:
	FSkyBoxPipeline(FDiligentRenderer* Renderer);
	~FSkyBoxPipeline();

	FSkyBoxVSHConstants SkyBoxVSHConstants;

	void Render(double RunningTime,
		FCamera* Camera,
		FSkyBoxDrawable* SkyBoxDrawable);
};
