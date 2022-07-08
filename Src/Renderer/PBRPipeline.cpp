#include "PBRPipeline.hpp"
#include "Util/Util.hpp"

FPBRPipeline::FPBRPipeline(FDiligentRenderer * Renderer)
	:Renderer(Renderer)
{
	CreateResources();
}

FPBRPipeline::~FPBRPipeline()
{
}

void FPBRPipeline::CreateResources()
{
	PBRPSHConstants = FPBRPSHConstantsDefault();
	PBRPSHConstants.DirectionalLight.Direction = glm::vec3(0.0f, 0.0f, 1.0f);

	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 1, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 2, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 3, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 4, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}

	Diligent::PipelineResourceLayoutDesc ResourceLayout;
	std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs;
	std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers;
	std::vector<std::string> TextureNames = {
		"AlbedoTexture",
		"NormalTexture",
		"MetallicTexture",
		"RoughnessTexture",
		"AmbientOcclusionTexture",
	};
	Renderer->PipelineResourceLayoutDesc(
		Diligent::SHADER_TYPE_PIXEL,
		&TextureNames,
		&ResourceLayout,
		&ShaderResourceVariableDescs,
		&ImtblSamplers);

	PSO = Renderer->DefaultPipelineState("PBR_PipelineState",
		"PBR.vsh",
		"PBR.psh",
		InputLayoutDesc,
		ResourceLayout);

	VSConstants = Renderer->CreateConstantsBuffer("PBRVSHConstants",
		nullptr,
		sizeof(FPBRVSHConstants));

	PSConstants = Renderer->CreateConstantsBuffer("PBRPSHConstants",
		nullptr,
		sizeof(FPBRPSHConstants));

	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "PBRVSHConstants"))
	{
		ResourceVariable->Set(VSConstants);
	}
	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_PIXEL, "PBRPSHConstants"))
	{
		ResourceVariable->Set(PSConstants);
	}
	PSO->CreateShaderResourceBinding(&SRB, true);
}

void FPBRPipeline::Render(
	FCamera* Camera,
	FPBRStaticMeshDrawable* PBRStaticMeshDrawable)
{
	assert(PBRStaticMeshDrawable);
	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&PBRVSHConstants,
		VSConstants);

	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&PBRPSHConstants,
		PSConstants);

	Renderer->SetPipelineState(PSO);

	for (FSubStaticMeshDrawableData& SubStaticMeshDrawableData : PBRStaticMeshDrawable->GetSubStaticMeshDrawableDatas())
	{
		std::unordered_map<std::string, aiTextureType> TypesDic;
		TypesDic["AlbedoTexture"] = aiTextureType::aiTextureType_DIFFUSE;
		TypesDic["NormalTexture"] = aiTextureType::aiTextureType_NORMALS;
		TypesDic["MetallicTexture"] = aiTextureType::aiTextureType_METALNESS;
		TypesDic["RoughnessTexture"] = aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS;
		TypesDic["AmbientOcclusionTexture"] = aiTextureType::aiTextureType_AMBIENT_OCCLUSION;
		for (const auto& Args : TypesDic)
		{
			if (Diligent::IShaderResourceVariable * ResourceVariable =
				SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, Args.first.c_str()))
			{
				Diligent::ITextureView* DiffuseTextureView = FUtil::Back<Diligent::ITextureView*>({
					SubStaticMeshDrawableData.GetTextureView(Args.second),
					Renderer->GetDefaultTexture2D()->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE)
					});
				if (DiffuseTextureView)
				{
					ResourceVariable->Set(DiffuseTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
				}
			}
		}

		Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer = SubStaticMeshDrawableData.VertexBuffer;
		Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer = SubStaticMeshDrawableData.IndexBuffer;
		const unsigned long long offset = 0;
		Diligent::IBuffer*     MultipleBuffers[] = { VertexBuffer };
		Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, MultipleBuffers, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
		Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::DrawIndexedAttribs DrawAttrs = PBRStaticMeshDrawable->GetDrawAttrs(SubStaticMeshDrawableData);
		Renderer->DrawIndexed(DrawAttrs);
	}

}
