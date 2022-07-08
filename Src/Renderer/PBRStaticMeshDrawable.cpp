#include "PBRStaticMeshDrawable.hpp"
#include "Util/Util.hpp"
#include "BridgingModels/PBRStaticMeshVertex.hpp"

FPBRStaticMeshDrawable::FPBRStaticMeshDrawable(FDiligentRenderer * Renderer,
	const FStaticMesh * StaticMesh)
	:StaticMesh(StaticMesh)
{
	CreateResource(Renderer);
}

FPBRStaticMeshDrawable::FPBRStaticMeshDrawable(
	std::vector<FSubStaticMeshDrawableData> SubStaticMeshDrawableDatas)
	: SubStaticMeshDrawableDatas(SubStaticMeshDrawableDatas)
{
}

FPBRStaticMeshDrawable::~FPBRStaticMeshDrawable()
{
}

std::vector<FSubStaticMeshDrawableData> FPBRStaticMeshDrawable::GetSubStaticMeshDrawableDatas() const
{
	return SubStaticMeshDrawableDatas;
}

Diligent::DrawIndexedAttribs FPBRStaticMeshDrawable::GetDrawAttrs(const FSubStaticMeshDrawableData& SubStaticMeshDrawableData) const noexcept
{
	Diligent::DrawIndexedAttribs DrawAttrs;
	DrawAttrs.IndexType = Diligent::VT_UINT32;
	DrawAttrs.NumIndices = SubStaticMeshDrawableData.IndexCount;
	DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	return DrawAttrs;
}

const FStaticMesh * FPBRStaticMeshDrawable::GetStaticMesh() const noexcept
{
	return StaticMesh;
}

void FPBRStaticMeshDrawable::CreateResource(FDiligentRenderer * Renderer)
{
	SubStaticMeshDrawableDatas.clear();
	const std::vector<const FStaticSubMesh*> SubMeshs = StaticMesh->GetSubMeshs();
	for (const auto& SubMesh : SubMeshs)
	{
		FSubStaticMeshDrawableData SubStaticMeshDrawableData;
		std::vector<FPBRStaticMeshVertex> Vertices;
		for (const FMeshVertex& MeshVertex : SubMesh->Vertices)
		{
			FPBRStaticMeshVertex PBRStaticMeshVertex;
			PBRStaticMeshVertex.Normal = MeshVertex.Normal;
			PBRStaticMeshVertex.Position = MeshVertex.Position;
			PBRStaticMeshVertex.TexCoords = MeshVertex.TexCoords;
			PBRStaticMeshVertex.Bitangents = MeshVertex.Bitangents;
			PBRStaticMeshVertex.Tangents = MeshVertex.Tangents;
			Vertices.push_back(PBRStaticMeshVertex);
		}
		SubStaticMeshDrawableData.VertexBuffer = Renderer->VertexBuffer("VertexBuffer0", Vertices.data(), Vertices.size(), sizeof(FPBRStaticMeshVertex));
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
