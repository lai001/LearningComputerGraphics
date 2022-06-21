#include "PhongShadingPSHConstantsResource.hpp"
#include "Util/Util.hpp"

FPhongShadingPSHConstantsResource::FPhongShadingPSHConstantsResource(FDiligentRenderer * Renderer)
	:Renderer(Renderer)
{
	Constants = Renderer->CreateConstantsBuffer("FPhongShadingPSHConstants",
		nullptr,
		sizeof(FPhongShadingPSHConstants));
}

Diligent::RefCntAutoPtr<Diligent::IBuffer> FPhongShadingPSHConstantsResource::GetConstants()
{
	return Constants;
}

void FPhongShadingPSHConstantsResource::Map()
{
	FUtil::CopyDataToBuffer(Renderer->GetImmediateContext(),
		&PhongShadingPSHConstants,
		Constants);
}
