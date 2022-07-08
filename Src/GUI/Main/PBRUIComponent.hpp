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

	virtual void Draw() override
	{
		if (!PBRPSHConstants) { return; }
		ImGui::Text(GetName().c_str());
		ImGui::DragFloat("RoughnessFactor", &PBRPSHConstants->RoughnessFactor, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("MetalnessFactor", &PBRPSHConstants->MetalnessFactor, 0.01f, 0.0f, 1.0f);
	}

	virtual std::string GetName() override
	{
		return "PBRUIComponent";
	}
};
