#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "Object/DirectionLightShadowMapInfo.hpp"

struct FDirectionLightDepthMapUIComponent : public IObjectPropertyUIComponent
{

	FDirectionLightShadowMapInfo* DirectionLightShadowMapInfo;

	virtual void Draw() override
	{
		ImGui::Text(GetName().c_str());
		
		ImGui::DragFloat("Left", &DirectionLightShadowMapInfo->Left, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("Right", &DirectionLightShadowMapInfo->Right, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("Top", &DirectionLightShadowMapInfo->Top, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("Bottom", &DirectionLightShadowMapInfo->Bottom, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("NearZ", &DirectionLightShadowMapInfo->NearZ, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat("FarZ", &DirectionLightShadowMapInfo->FarZ, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat3("Eye", glm::value_ptr(DirectionLightShadowMapInfo->Eye), 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Center", glm::value_ptr(DirectionLightShadowMapInfo->Center), 0.1f, -360.0f, 360.0f);
	}

	virtual std::string GetName() override
	{
		return "DirectionLightDepthMapUIComponent";
	}
};