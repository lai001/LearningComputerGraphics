#include "StaticMeshPipeline.hpp"
#include "Util/Util.hpp"

FStaticMeshPipeline::FStaticMeshPipeline(FDiligentRenderer* Renderer,
	FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource)
	:Renderer(Renderer)
{
	CreateResources(PhongShadingPSHConstantsResource);
}

FStaticMeshPipeline::~FStaticMeshPipeline()
{
}

void FStaticMeshPipeline::Render(const FRenderInfo& RenderInfo)
{
	assert(RenderInfo.Camera);
	assert(RenderInfo.StaticMeshDrawable);
	const FStaticMesh *StaticMesh = RenderInfo.StaticMeshDrawable->GetStaticMesh();
	assert(StaticMesh);
	{
		PhongShadingVSHConstants.Model = StaticMesh->GetModelMatrix();
		PhongShadingVSHConstants.View = RenderInfo.Camera->GetViewMat();
		PhongShadingVSHConstants.Projection = RenderInfo.Camera->GetprojectionMat();
	}

	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&PhongShadingVSHConstants,
		VSConstants);

	Renderer->SetPipelineState(PSO);

	for (FSubStaticMeshDrawableData& SubStaticMeshDrawableData : RenderInfo.StaticMeshDrawable->GetSubStaticMeshDrawableDatas())
	{
		if (Diligent::IShaderResourceVariable * ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "DiffuseTexture"))
		{
			Diligent::ITextureView* DiffuseTextureView = FUtil::Back<Diligent::ITextureView*>({
				SubStaticMeshDrawableData.GetTextureView(aiTextureType::aiTextureType_DIFFUSE),
				Renderer->GetDefaultTexture2D()->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE)
			});
			if (DiffuseTextureView)
			{
				ResourceVariable->Set(DiffuseTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
			}
		}
		if (Diligent::IShaderResourceVariable * ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "SpecularTexture"))
		{
			Diligent::ITextureView* SpecularTextureView = FUtil::Back<Diligent::ITextureView*>({
				SubStaticMeshDrawableData.GetTextureView(aiTextureType::aiTextureType_SPECULAR),
				 Renderer->GetDefaultTexture2D()->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE)
			});
			if (SpecularTextureView)
			{
				ResourceVariable->Set(SpecularTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
			}
		}
		if (Diligent::IShaderResourceVariable * ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "DirctionLightShadowMapTexture"))
		{
			Diligent::RefCntAutoPtr<Diligent::ITextureView> DirctionLightShadowMap =
				RenderInfo.DirctionLightShadowMap;
			ResourceVariable->Set(DirctionLightShadowMap, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
		}

		Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer = SubStaticMeshDrawableData.VertexBuffer;
		Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer = SubStaticMeshDrawableData.IndexBuffer;
		const unsigned long long offset = 0;
		Diligent::IBuffer*     pBuffs[] = { VertexBuffer };
		Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
		Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::DrawIndexedAttribs DrawAttrs = RenderInfo.StaticMeshDrawable->GetDrawAttrs(SubStaticMeshDrawableData);
		Renderer->DrawIndexed(DrawAttrs);
	}
}

void FStaticMeshPipeline::CreateResources(FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource)
{
	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 1, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 2, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}

	Diligent::PipelineResourceLayoutDesc ResourceLayout;
	std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs;
	std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers;
	std::vector<std::string> TextureNames = {
		"DiffuseTexture",
		"SpecularTexture",
		"DirctionLightShadowMapTexture"
	};
	Renderer->PipelineResourceLayoutDesc(
		Diligent::SHADER_TYPE_PIXEL,
		&TextureNames,
		&ResourceLayout,
		&ShaderResourceVariableDescs,
		&ImtblSamplers);

	PSO = Renderer->DefaultPipelineState("StaticMeshPipelineState",
		"StaticMeshPhongShading.vsh",
		"StaticMeshPhongShading.psh",
		InputLayoutDesc,
		ResourceLayout);

	VSConstants = Renderer->CreateConstantsBuffer("PhongShadingVSHConstants",
		nullptr,
		sizeof(FPhongShadingVSHConstants));

	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "PhongShadingVSHConstants"))
	{
		ResourceVariable->Set(VSConstants);
	}
	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_PIXEL, "FPhongShadingPSHConstants"))
	{
		ResourceVariable->Set(PhongShadingPSHConstantsResource->GetConstants());
	}
	PSO->CreateShaderResourceBinding(&SRB, true);
}
