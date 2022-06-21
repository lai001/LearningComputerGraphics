#include "SkeletonMeshDrawable.hpp"
#include "Util/Util.hpp"

FSkeletonMeshDrawable::FSkeletonMeshDrawable(FDiligentRenderer * renderer, 
	const FSkeletonMesh * SkeletonMesh)
	:SkeletonMesh(SkeletonMesh)
{
	CreateResource(renderer);
}

FSkeletonMeshDrawable::~FSkeletonMeshDrawable()
{
}

void FSkeletonMeshDrawable::CreateResource(FDiligentRenderer * Renderer)
{
	SubStaticMeshDrawableDatas.clear();
	const std::vector<const FSkeletionSubMesh*> SubMeshs = SkeletonMesh->GetSubMeshs();
	for (const auto& SubMesh : SubMeshs)
	{
		FSubStaticMeshDrawableData SubStaticMeshDrawableData;
		SubStaticMeshDrawableData.VertexBuffer = Renderer->VertexBuffer("VertexBuffer0", SubMesh->Vertices.data(), SubMesh->Vertices.size(), sizeof(FVertex));
		SubStaticMeshDrawableData.IndexBuffer = Renderer->IndexBuffer("IndexBuffer0", SubMesh->Indices.data(), SubMesh->Indices.size());
		SubStaticMeshDrawableData.IndexCount = SubMesh->Indices.size();
		for (FTextureDescription* TextureDescription : SubMesh->Textures)
		{
			const ks::PixelBuffer * PixelBuffer = TextureDescription->GetPixelBuffer();
			assert(PixelBuffer->getType() == ks::PixelBuffer::FormatType::rgba8);
			Diligent::RefCntAutoPtr<Diligent::ITexture> TextureResource;
			TextureResource = Renderer->ToTexture2D(TextureDescription->GetFilepath(), PixelBuffer);
			SubStaticMeshDrawableData.TextureDic[TextureDescription->GetTextureType()] = TextureResource;
		}
		SubStaticMeshDrawableDatas.push_back(SubStaticMeshDrawableData);
	}
}

std::vector<FSubStaticMeshDrawableData> FSkeletonMeshDrawable::GetSubStaticMeshDrawableDatas() const
{
	return SubStaticMeshDrawableDatas;
}

Diligent::DrawIndexedAttribs FSkeletonMeshDrawable::GetDrawAttrs(const FSubStaticMeshDrawableData & SubStaticMeshDrawableData) const noexcept
{
	Diligent::DrawIndexedAttribs DrawAttrs;
	DrawAttrs.IndexType = Diligent::VT_UINT32;
	DrawAttrs.NumIndices = SubStaticMeshDrawableData.IndexCount;
	DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	return DrawAttrs;
}

const FSkeletonMesh * FSkeletonMeshDrawable::GetSkeletonMesh() const noexcept
{
	return SkeletonMesh;
}
