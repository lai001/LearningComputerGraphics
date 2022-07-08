#include "SkeletonMeshPipeline.hpp"
#include "Util/Util.hpp"

FSkeletonMeshPipeline::FSkeletonMeshPipeline(FDiligentRenderer* Renderer,
	FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource)
	:Renderer(Renderer)
{
	CreateResources(PhongShadingPSHConstantsResource);
}

FSkeletonMeshPipeline::~FSkeletonMeshPipeline()
{
}

void FSkeletonMeshPipeline::CreateResources(FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource)
{
	Diligent::InputLayoutDesc InputLayoutDesc;
	std::vector<Diligent::LayoutElement> LayoutElements;
	{
		LayoutElements.push_back(Diligent::LayoutElement{ 0, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 1, 0, 3, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 2, 0, 2, Diligent::VT_FLOAT32, Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 3, 0, 4, Diligent::VT_INT32,	 Diligent::False });
		LayoutElements.push_back(Diligent::LayoutElement{ 4, 0, 4, Diligent::VT_FLOAT32, Diligent::False });
		InputLayoutDesc.LayoutElements = LayoutElements.data();
		InputLayoutDesc.NumElements = LayoutElements.size();
	}

	Diligent::PipelineResourceLayoutDesc ResourceLayout;
	std::vector<Diligent::ShaderResourceVariableDesc> ShaderResourceVariableDescs;
	std::vector<Diligent::ImmutableSamplerDesc> ImtblSamplers;
	std::vector<std::string> TextureNames = {
		"DiffuseTexture",
		"SpecularTexture"
	};
	Renderer->PipelineResourceLayoutDesc(
		Diligent::SHADER_TYPE_PIXEL,
		&TextureNames,
		&ResourceLayout,
		&ShaderResourceVariableDescs,
		&ImtblSamplers);

	PSO = Renderer->DefaultPipelineState("SkeletonMeshPipelineState",
		"SkeletonMeshPhongShading.vsh",
		"SkeletonMeshPhongShading.psh",
		InputLayoutDesc,
		ResourceLayout);

	VSConstants = Renderer->CreateConstantsBuffer("SkeletonMeshPhongShadingVSHConstants",
		nullptr,
		sizeof(FSkeletonMeshPhongShadingVSHConstants));

	if (Diligent::IShaderResourceVariable * ResourceVariable =
		PSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "SkeletonMeshPhongShadingVSHConstants"))
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

void FSkeletonMeshPipeline::Render(double RunningTime,
	FCamera* Camera,
	FSkeletonMeshDrawable* SkeletonMeshDrawable)
{
	assert(Camera);
	assert(SkeletonMeshDrawable);
	const FSkeletonMesh  *SkeletonMesh = SkeletonMeshDrawable->GetSkeletonMesh();
	assert(SkeletonMesh);
	{
		SkeletonMeshPhongShadingVSHConstants.Model = SkeletonMesh->GetModelMatrix();
		SkeletonMeshPhongShadingVSHConstants.View = Camera->GetViewMat();
		SkeletonMeshPhongShadingVSHConstants.Projection = Camera->GetProjectionMat();
		const std::vector<glm::mat4> BonesTransforms = SkeletonMesh->GetTransforms();
		memcpy(SkeletonMeshPhongShadingVSHConstants.Bones,
			BonesTransforms.data(),
			BonesTransforms.size() * sizeof(glm::mat4));
	}

	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&SkeletonMeshPhongShadingVSHConstants,
		VSConstants);

	Renderer->SetPipelineState(PSO);

	for (FSubStaticMeshDrawableData& SubStaticMeshDrawableData : SkeletonMeshDrawable->GetSubStaticMeshDrawableDatas())
	{
		Diligent::RefCntAutoPtr<Diligent::ITextureView> DiffuseTextureView =
			SubStaticMeshDrawableData.GetTextureView(aiTextureType::aiTextureType_DIFFUSE);
		if (Diligent::IShaderResourceVariable * ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "DiffuseTexture"))
		{
			if (DiffuseTextureView)
			{
				ResourceVariable->Set(DiffuseTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
			}
		}
		if (Diligent::IShaderResourceVariable * ResourceVariable =
			SRB->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "SpecularTexture"))
		{
			Diligent::RefCntAutoPtr<Diligent::ITextureView> SpecularTextureView =
				SubStaticMeshDrawableData.GetTextureView(aiTextureType::aiTextureType_SPECULAR);
			if (SpecularTextureView)
			{
				ResourceVariable->Set(SpecularTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
			}
			else
			{
				ResourceVariable->Set(DiffuseTextureView, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
			}
		}
		Renderer->GetImmediateContext()->CommitShaderResources(SRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer = SubStaticMeshDrawableData.VertexBuffer;
		Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer = SubStaticMeshDrawableData.IndexBuffer;
		const unsigned long long offset = 0;
		Diligent::IBuffer*     pBuffs[] = { VertexBuffer };
		Renderer->GetImmediateContext()->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
		Renderer->GetImmediateContext()->SetIndexBuffer(IndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		Diligent::DrawIndexedAttribs DrawAttrs = SkeletonMeshDrawable->GetDrawAttrs(SubStaticMeshDrawableData);
		Renderer->DrawIndexed(DrawAttrs);
	}
}
