#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "BridgingModels/PhongShadingPSHConstants.hpp"

struct FPointLightUIComponent : public IObjectPropertyUIComponent
{
	FPointLight* PointLight = nullptr;

	virtual void Draw() override
	{
		assert(PointLight);
		ImGui::DragFloat3("Position", glm::value_ptr(PointLight->Position), 0.1f, -100.0, 100.0);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(PointLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(PointLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(PointLight->Specular), -100.0, 100.0);
		ImGui::SliderFloat("Constant", &PointLight->Constant, 1.0, 100.0);
		ImGui::SliderFloat("Linear", &PointLight->Linear, 1.0, 100.0);
		ImGui::SliderFloat("Quadratic", &PointLight->Quadratic, 1.0, 100.0);
		bool isEnable = PointLight->isEnable;
		ImGui::Checkbox("is enable", &isEnable);
		PointLight->isEnable = isEnable;
	}

	virtual std::string GetName() override
	{
		return "PointLight";
	}
};
