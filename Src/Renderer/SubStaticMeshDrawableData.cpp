#include "SubStaticMeshDrawableData.hpp"

Diligent::RefCntAutoPtr<Diligent::ITextureView> FSubStaticMeshDrawableData::GetTextureView(const aiTextureType TextureType)
{
	if (TextureDic.end() == TextureDic.find(TextureType))
	{
		return Diligent::RefCntAutoPtr<Diligent::ITextureView>();
	}
	else
	{
		Diligent::RefCntAutoPtr<Diligent::ITextureView> TextureSRV;
		TextureSRV = TextureDic.at(TextureType)->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
		return TextureSRV;
	}
}
