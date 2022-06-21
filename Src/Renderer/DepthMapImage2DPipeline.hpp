#pragma once

#include <array>
#include "DiligentRenderer.hpp"
#include "BridgingModels/ImageVertex.hpp"

class FDepthMapImage2DPipeline
{
private:
	FDiligentRenderer* Renderer = nullptr;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> PSO;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> SRB;

	void CreateResource(FDiligentRenderer* Renderer);

public:
	FDepthMapImage2DPipeline(FDiligentRenderer* Renderer);
	~FDepthMapImage2DPipeline();

	void Render(
		const std::array<glm::vec2, 4>& Vertexs,
		Diligent::RefCntAutoPtr<Diligent::ITexture> Texture);
};
