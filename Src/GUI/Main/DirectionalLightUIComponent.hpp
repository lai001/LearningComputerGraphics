#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "BridgingModels/PhongShadingPSHConstants.hpp"
#include "Object/StaticMesh.hpp"

struct FDirectionalLightUIComponent : public IObjectPropertyUIComponent
{
	FDirectionalLight* DirectionalLight = nullptr;

	virtual void Draw() override
	{
		assert(DirectionalLight);
		ImGui::Text(GetName().c_str());
		ImGui::SliderFloat3("rotation", glm::value_ptr(DirectionalLight->Direction), -360.0f, 360.0f);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(DirectionalLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(DirectionalLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(DirectionalLight->Specular), -100.0, 100.0);
		bool isEnable = DirectionalLight->isEnable;
		ImGui::Checkbox("is enable", &isEnable);
		DirectionalLight->isEnable = isEnable;
	}

	virtual std::string GetName() override
	{
		return "DirectionalLight";
	}
};
