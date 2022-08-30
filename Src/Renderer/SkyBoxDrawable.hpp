#pragma once
#include "Object/SkyBox.hpp"
#include "DiligentRenderer.hpp"

class FSkyBoxDrawable: public ks::noncopyable
{
private:
	FDiligentRenderer* Renderer = nullptr;
	FSkyBox* SkyBox = nullptr;
	Diligent::RefCntAutoPtr<Diligent::ITexture> TextureCube;

	Diligent::RefCntAutoPtr<Diligent::IBuffer> VertexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> IndexBuffer;
	unsigned int IndexCount = 0;
	void CreateResource(FDiligentRenderer* Renderer);

public:
	FSkyBoxDrawable(FDiligentRenderer* Renderer, FSkyBox* SkyBox);
	FSkyBoxDrawable(FDiligentRenderer* Renderer, Diligent::RefCntAutoPtr<Diligent::ITexture> TextureCube);

	Diligent::RefCntAutoPtr<Diligent::ITextureView> GetTextureView() noexcept;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> GetVertexBuffer();
	Diligent::RefCntAutoPtr<Diligent::IBuffer> GetIndexBuffer();
	Diligent::DrawIndexedAttribs GetDrawAttrs() const noexcept;
};
