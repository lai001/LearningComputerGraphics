#include "StaticMeshDrawable.hpp"
#include "Util/Util.hpp"

FStaticMeshDrawable::FStaticMeshDrawable(FDiligentRenderer * Renderer,
	const FStaticMesh * StaticMesh)
	:StaticMesh(StaticMesh)
{
	CreateResource(Renderer);
}

FStaticMeshDrawable::FStaticMeshDrawable(
	std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas)
	: SubStaticMeshDrawableDatas(SubStaticMeshDrawableDatas)
{
}

FStaticMeshDrawable::~FStaticMeshDrawable()
{
}

std::vector<FSubStaticMeshDrawableData> FStaticMeshDrawable::GetSubStaticMeshDrawableDatas() const
{
	return SubStaticMeshDrawableDatas;
}

Diligent::DrawIndexedAttribs FStaticMeshDrawable::GetDrawAttrs(const FSubStaticMeshDrawableData& SubStaticMeshDrawableData) const noexcept
{
	Diligent::DrawIndexedAttribs DrawAttrs;
	DrawAttrs.IndexType = Diligent::VT_UINT32;
	DrawAttrs.NumIndices = SubStaticMeshDrawableData.IndexCount;
	DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	return DrawAttrs;
}

const FStaticMesh * FStaticMeshDrawable::GetStaticMesh() const noexcept
{
	return StaticMesh;
}

void FStaticMeshDrawable::CreateResource(FDiligentRenderer * Renderer)
{
	SubStaticMeshDrawableDatas.clear();
	const std::vector<const FStaticSubMesh*> SubMeshs = StaticMesh->GetSubMeshs();
	for (const auto& SubMesh : SubMeshs)
	{
		FSubStaticMeshDrawableData SubStaticMeshDrawableData;
		std::vector<FBaseVertex> Vertices;
		for (const FMeshVertex& MeshVertex : SubMesh->Vertices)
		{
			FBaseVertex BaseVertex;
			BaseVertex.Normal = MeshVertex.Normal;
			BaseVertex.Position = MeshVertex.Position;
			BaseVertex.TexCoords = MeshVertex.TexCoords;
			Vertices.push_back(BaseVertex);
		}
		SubStaticMeshDrawableData.VertexBuffer = Renderer->VertexBuffer("VertexBuffer0", Vertices.data(), Vertices.size(), sizeof(FBaseVertex));
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
