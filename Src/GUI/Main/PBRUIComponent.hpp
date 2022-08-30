#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "BridgingModels/PBRPSHConstants.hpp"

struct FPBRUIComponent : public IObjectPropertyUIComponent
{
	FPBRPSHConstants* PBRPSHConstants = nullptr;
	int CurrentItemIndex = 0;

	virtual void Draw() override
	{
		if (!PBRPSHConstants) { return; }
		ImGui::Text(GetName().c_str());
		ImGui::DragFloat("RoughnessFactor", &PBRPSHConstants->RoughnessFactor, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("MetalnessFactor", &PBRPSHConstants->MetalnessFactor, 0.01f, 0.0f, 1.0f);

		bool isDebugViewEnable = PBRPSHConstants->isDebugViewEnable;

		ImGui::Checkbox("IsDebugViewEnable", &isDebugViewEnable);

		PBRPSHConstants->isDebugViewEnable = isDebugViewEnable;
		PBRPSHConstants->isDebugAlbedoTexture = 0;
		PBRPSHConstants->isDebugMetallicTexture = 0;
		PBRPSHConstants->isDebugNormalTexture = 0;
		PBRPSHConstants->isDebugRoughnessTexture = 0;
		PBRPSHConstants->isDebugIBLView = 0;

		if (isDebugViewEnable)
		{
			const char* Items[] = {
				"isDebugRoughnessTexture",
				"isDebugMetallicTexture",
				"isDebugAlbedoTexture",
				"isDebugNormalTexture",
				"isDebugIBLView",
			};

			int* Values[] = { 
				&PBRPSHConstants->isDebugRoughnessTexture,
				&PBRPSHConstants->isDebugMetallicTexture,
				&PBRPSHConstants->isDebugAlbedoTexture,
				&PBRPSHConstants->isDebugNormalTexture,
				&PBRPSHConstants->isDebugIBLView,
			};
			ImGui::Combo("DebugType", &CurrentItemIndex, Items, IM_ARRAYSIZE(Items));
			for (int Index = 0; Index < IM_ARRAYSIZE(Values); Index++)
			{
				if (CurrentItemIndex == Index)
				{
					*Values[Index] = 1;
				}
			}
		}
	}

	virtual std::string GetName() override
	{
		return "PBRUIComponent";
	}
};
