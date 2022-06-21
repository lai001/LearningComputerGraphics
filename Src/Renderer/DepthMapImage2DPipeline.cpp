#include "DepthMapImage2DPipeline.hpp"

void FDepthMapImage2DPipeline::CreateResource(FDiligentRenderer * Renderer)
{
	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 1, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}

	Diligent::PipelineResourceLayoutDesc ResourceLayout;
	std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs;
	std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers;
	std::vector<std::string> TextureNames = {
		"Image2DTexture"
	};
	Renderer->PipelineResourceLayoutDesc(
		Diligent::SHADER_TYPE_PIXEL,
		&TextureNames,
		&ResourceLayout,
		&ShaderResourceVariableDescs,
		&ImtblSamplers);

	Diligent::GraphicsPipelineDesc GraphicsPipelineDesc = Renderer->DefaultGraphicsPipelineDesc();
	GraphicsPipelineDesc.DepthStencilDesc.DepthEnable = false;
	GraphicsPipelineDesc.InputLayout = InputLayoutDesc;
	PSO = Renderer->PipelineState("DepthMapImage2DPipelineState",
		"DepthMapImage2D.vsh",
		"DepthMapImage2D.psh",
		ResourceLayout,
		GraphicsPipelineDesc);
	PSO->CreateShaderResourceBinding(&SRB, true);
}

FDepthMapImage2DPipeline::FDepthMapImage2DPipeline(FDiligentRenderer * Renderer)
	:Renderer(Renderer)
{
	CreateResource(Renderer);
}

FDepthMapImage2DPipeline::~FDepthMapImage2DPipeline()
{
}

void FDepthMapImage2DPipeline::Render(
	const std::array<glm::vec2, 4>& Vertexs,
	Diligent::RefCntAutoPtr<Diligent::ITexture> Texture)
{
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer;
	std::array<unsigned int, 6> Indices = {
		0, 1, 2,
		2, 1, 3
	};
	std::array<glm::vec2, 4> TexCoords = {
		glm::vec2(0.0, 0.0),
		glm::vec2(1.0, 0.0),
		glm::vec2(0.0, 1.0),
		glm::vec2(1.0, 1.0),
	};
	std::array<FImageVertex, 4> ImageVertexs;
	for (size_t i = 0; i < ImageVertexs.size(); i++)
	{
		ImageVertexs[i].Position = Vertexs[i];
		ImageVertexs[i].TexCoords = TexCoords[i];
	}

	VertexBuffer = Renderer->VertexBuffer("Image2DVertexBuffer",
		ImageVertexs.data(),
		ImageVertexs.size(),
		sizeof(FImageVertex));
	IndexBuffer = Renderer->IndexBuffer("Image2DIndexBuffer",
		Indices.data(),
		Indices.size());
	Renderer->SetPipelineState(PSO);
	
	if (Diligent::IShaderResourceVariable * SRV = 
		SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Image2DTexture"))
	{
		Diligent::ITextureView * TextureView =
			Texture->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE);
		assert(TextureView);
		SRV->Set(TextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
	}

	const unsigned long long offset = 0;
	Diligent::IBuffer*  Buffers[] = { VertexBuffer };
	Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, Buffers, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
	Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	Diligent::DrawIndexedAttribs DrawAttrs;
	DrawAttrs.NumIndices = Indices.size();
	DrawAttrs.IndexType = Diligent::VALUE_TYPE::VT_UINT32;
	DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	Renderer->DrawIndexed(DrawAttrs);
}
