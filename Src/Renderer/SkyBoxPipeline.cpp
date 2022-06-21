#include "SkyBoxPipeline.hpp"
#include "Util/Util.hpp"

void FSkyBoxPipeline::CreateResources()
{
	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}

	Diligent::PipelineResourceLayoutDesc ResourceLayout;
	std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs;
	std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers;
	{
		ShaderResourceVariableDescs.push_back({ Diligent::SHADER_TYPE_PIXEL, "SkyBoxTexture", Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });
		Diligent::SamplerDesc SamLinearClampDesc
		{
			Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR,
			Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP
		};
		ImtblSamplers.push_back({ Diligent::SHADER_TYPE_PIXEL, "SkyBoxTexture", SamLinearClampDesc });
		ResourceLayout.DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		ResourceLayout.Variables = ShaderResourceVariableDescs.data();
		ResourceLayout.NumVariables = ShaderResourceVariableDescs.size();
		ResourceLayout.ImmutableSamplers = ImtblSamplers.data();
		ResourceLayout.NumImmutableSamplers = ImtblSamplers.size();
	}
	Diligent::GraphicsPipelineDesc GraphicsPipelineDesc = 
		Renderer->DefaultGraphicsPipelineDesc();
	GraphicsPipelineDesc.DepthStencilDesc.DepthFunc = Diligent::COMPARISON_FUNC_LESS_EQUAL;
	GraphicsPipelineDesc.InputLayout = InputLayoutDesc;
	PSO = Renderer->PipelineState("SkyBoxPipelineState",
		"SkyBox.vsh",
		"SkyBox.psh",
		ResourceLayout,
		GraphicsPipelineDesc);

	VSConstants = Renderer->CreateConstantsBuffer("SkyBoxVSHConstants",
		nullptr,
		sizeof(FSkyBoxVSHConstants));

	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "SkyBoxVSHConstants"))
	{
		ResourceVariable->Set(VSConstants);
	}

	PSO->CreateShaderResourceBinding(&SRB, true);
}

FSkyBoxPipeline::FSkyBoxPipeline(FDiligentRenderer * Renderer)
	:Renderer(Renderer)
{
	CreateResources();
}

FSkyBoxPipeline::~FSkyBoxPipeline()
{
}

void FSkyBoxPipeline::Render(double RunningTime,
	FCamera * Camera,
	FSkyBoxDrawable * SkyBoxDrawable)
{
	assert(Camera);
	
	SkyBoxVSHConstants.View = glm::mat4(glm::mat3(Camera->GetViewMat()));
	SkyBoxVSHConstants.Projection = Camera->GetprojectionMat();

	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&SkyBoxVSHConstants,
		VSConstants);

	Renderer->SetPipelineState(PSO);

	if (Diligent::IShaderResourceVariable * ResourceVariable =
		SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "SkyBoxTexture"))
	{
		Diligent::RefCntAutoPtr<Diligent::ITextureView> TextureView = 
			SkyBoxDrawable->GetTextureView();
		if (TextureView)
		{
			ResourceVariable->Set(TextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
		}
	}

	Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	
	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer = SkyBoxDrawable->GetVertexBuffer();
	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer = SkyBoxDrawable->GetIndexBuffer();
	const unsigned long long offset = 0;
	Diligent::IBuffer*  Buffers[] = { VertexBuffer };
	Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, Buffers, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
	Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	
	Diligent::DrawIndexedAttribs DrawAttrs = SkyBoxDrawable->GetDrawAttrs();
	Renderer->DrawIndexed(DrawAttrs);
}
