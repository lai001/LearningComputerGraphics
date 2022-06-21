#include "DirectionLightDepthMapPipeline.hpp"
#include "Util/Util.hpp"

void FDirectionLightDepthMapPipeline::CreateResources()
{
	VSConstants = Renderer->CreateConstantsBuffer("DirectionLightDepthMapVSHConstants",
		nullptr,
		sizeof(FDirectionLightDepthMapVSHConstants));

	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 1, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 2, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}
	Diligent::GraphicsPipelineDesc GraphicsPipelineDesc = Renderer->DefaultGraphicsPipelineDesc();
	GraphicsPipelineDesc.InputLayout = InputLayoutDesc;
	GraphicsPipelineDesc.NumRenderTargets = 0;
	GraphicsPipelineDesc.RTVFormats[0] = Diligent::TEX_FORMAT_UNKNOWN;
	GraphicsPipelineDesc.DSVFormat = Diligent::TEX_FORMAT_D16_UNORM;
	GraphicsPipelineDesc.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	GraphicsPipelineDesc.RasterizerDesc.CullMode = Diligent::CULL_MODE_BACK;
	GraphicsPipelineDesc.DepthStencilDesc.DepthEnable = Diligent::True;

	PSO = Renderer->PipelineState("DirectionLightDepthMapPipelineState",
		"DirectionLightDepthMap.vsh",
		"",
		Diligent::PipelineResourceLayoutDesc(),
		GraphicsPipelineDesc);
	PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "DirectionLightDepthMapVSHConstants")->Set(VSConstants);
	PSO->CreateShaderResourceBinding(&ShadowSRB, true);

	Diligent::TextureDesc TextureDesc;
	TextureDesc.ArraySize = 1;
	TextureDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.Format = Diligent::TEX_FORMAT_D16_UNORM;
	TextureDesc.BindFlags = Diligent::BIND_DEPTH_STENCIL | Diligent::BIND_SHADER_RESOURCE;
	Renderer->GetDevice()->CreateTexture(TextureDesc, nullptr, &Texture);
	ShadowMapSRV = Texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
	ShadowMapDSV = Texture->GetDefaultView(Diligent::TEXTURE_VIEW_DEPTH_STENCIL);
}

FDirectionLightDepthMapPipeline::FDirectionLightDepthMapPipeline(
	FDiligentRenderer* Renderer,
	const unsigned int Width,
	const unsigned int Height)
	:Renderer(Renderer), Width(Width), Height(Height)
{
	CreateResources();
}

FDirectionLightDepthMapPipeline::~FDirectionLightDepthMapPipeline()
{
}

void FDirectionLightDepthMapPipeline::Render(
	double RunningTime, 
	FStaticMeshDrawable * StaticMeshDrawable)
{
	assert(StaticMeshDrawable);
	const FStaticMesh *StaticMesh = StaticMeshDrawable->GetStaticMesh();
	assert(StaticMesh);
	{
		DirectionLightDepthMapVSHConstants.Model = StaticMesh->GetModelMatrix();
	}

	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&DirectionLightDepthMapVSHConstants,
		VSConstants);

	Renderer->SetPipelineState(PSO);

	for (FSubStaticMeshDrawableData& SubStaticMeshDrawableData : StaticMeshDrawable->GetSubStaticMeshDrawableDatas())
	{
		Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer = SubStaticMeshDrawableData.VertexBuffer;
		Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer = SubStaticMeshDrawableData.IndexBuffer;
		const unsigned long long offset = 0;
		Diligent::IBuffer*     pBuffs[] = { VertexBuffer };
		Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
		Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Renderer->GetImmediateContext()->CommitShaderResources(ShadowSRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Diligent::DrawIndexedAttribs DrawAttrs = StaticMeshDrawable->GetDrawAttrs(SubStaticMeshDrawableData);
		Renderer->DrawIndexed(DrawAttrs);
	}
}

void FDirectionLightDepthMapPipeline::NewFrame()
{
	Renderer->GetImmediateContext()->SetRenderTargets(0, nullptr, ShadowMapDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	Renderer->GetImmediateContext()->ClearDepthStencil(ShadowMapDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

Diligent::RefCntAutoPtr<Diligent::ITextureView> FDirectionLightDepthMapPipeline::GetRenderTargetView()
{
	return ShadowMapDSV;
}

Diligent::RefCntAutoPtr<Diligent::ITextureView> FDirectionLightDepthMapPipeline::GetResourceView()
{
	return ShadowMapSRV;
}

Diligent::RefCntAutoPtr<Diligent::ITexture> FDirectionLightDepthMapPipeline::GetTexture() noexcept
{
	return Texture;
}
