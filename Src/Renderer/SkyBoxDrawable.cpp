#include "SkyBoxDrawable.hpp"

FSkyBoxDrawable::FSkyBoxDrawable(FDiligentRenderer * Renderer,
	FSkyBox * SkyBox)
	:Renderer(Renderer), SkyBox(SkyBox)
{
	TextureCube = Renderer->CreateTextureCube("SkyBox",
		SkyBox->GetPixelBuffers());
	CreateResource(Renderer);
}

FSkyBoxDrawable::FSkyBoxDrawable(FDiligentRenderer* Renderer, Diligent::RefCntAutoPtr<Diligent::ITexture> TextureCube)
	: Renderer(Renderer), TextureCube(TextureCube)
{
	assert(TextureCube);
	CreateResource(Renderer);
}

Diligent::RefCntAutoPtr<Diligent::ITextureView> FSkyBoxDrawable::GetTextureView() noexcept
{
	if (TextureCube)
	{
		Diligent::RefCntAutoPtr<Diligent::ITextureView> TextureSRV;
		TextureSRV = TextureCube->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
		return TextureSRV;
	}
	else
	{
		assert(false);
		return Diligent::RefCntAutoPtr<Diligent::ITextureView>();
	}
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FSkyBoxDrawable::GetVertexBuffer()
{
	return VertexBuffer;
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FSkyBoxDrawable::GetIndexBuffer()
{
	return IndexBuffer;
}

Diligent::DrawIndexedAttribs FSkyBoxDrawable::GetDrawAttrs() const noexcept
{
	Diligent::DrawIndexedAttribs DrawAttrs;
	DrawAttrs.IndexType = Diligent::VT_UINT32;
	DrawAttrs.NumIndices = IndexCount;
	DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	return DrawAttrs;
}

void FSkyBoxDrawable::CreateResource(FDiligentRenderer * Renderer)
{
	std::vector<glm::vec3> Vertexs = {
		glm::vec3(-1.0f,  1.0f, -1.0f), 
		glm::vec3( 1.0f,  1.0f, -1.0f), 
		glm::vec3(-1.0f, -1.0f, -1.0f), 
		glm::vec3( 1.0f, -1.0f, -1.0f), 

		glm::vec3(-1.0f,  1.0f,  1.0f), 
		glm::vec3( 1.0f,  1.0f,  1.0f), 
		glm::vec3(-1.0f, -1.0f,  1.0f), 
		glm::vec3( 1.0f, -1.0f,  1.0f),
	};

	VertexBuffer = Renderer->VertexBuffer("SkyBox_VertexBuffer",
		Vertexs.data(),
		Vertexs.size(),
		sizeof(glm::vec3));

	std::vector<unsigned int> Indices = {
		1, 5, 3, 3, 5, 7,
		4, 0, 6, 6, 0, 2,
		4, 5, 0, 0, 5, 1,
		6, 7, 2, 2, 7, 3,
		4, 5, 6, 6, 5, 7,
		0, 1, 2, 2, 1, 3, 
	};

	IndexBuffer = Renderer->IndexBuffer("SkyBox_IndexBuffer",
		Indices.data(),
		Indices.size());

	IndexCount = Indices.size();
}
