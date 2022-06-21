#pragma once
#include <Foundation/Foundation.hpp>
#include "DiligentRenderer.hpp"
#include "BridgingModels/PhongShadingPSHConstants.hpp"

class FPhongShadingPSHConstantsResource: public ks::noncopyable
{
private:
	Diligent::RefCntAutoPtr<Diligent::IBuffer> Constants;
	FDiligentRenderer* Renderer = nullptr;

public:
	FPhongShadingPSHConstants PhongShadingPSHConstants =
		FPhongShadingPSHConstantsDefault();

	FPhongShadingPSHConstantsResource(FDiligentRenderer* Renderer);
	Diligent::RefCntAutoPtr<Diligent::IBuffer> GetConstants();
	void Map();
};
