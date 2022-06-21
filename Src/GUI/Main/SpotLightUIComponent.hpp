#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "BridgingModels/PhongShadingPSHConstants.hpp"

struct FSpotLightUIComponent : public IObjectPropertyUIComponent
{
	FSpotLight* SpotLight = nullptr;

	virtual void Draw() override
	{
		assert(SpotLight);
		ImGui::SliderFloat3("Position", glm::value_ptr(SpotLight->Position), -100.0, 100.0);
		ImGui::SliderFloat3("Direction", glm::value_ptr(SpotLight->Direction), -100.0, 100.0);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(SpotLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(SpotLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(SpotLight->Specular), -100.0, 100.0);
		ImGui::SliderFloat("Constant", &SpotLight->Constant, 1.0, 100.0);
		ImGui::SliderFloat("Linear", &SpotLight->Linear, 1.0, 100.0);
		ImGui::SliderFloat("Quadratic", &SpotLight->Quadratic, 1.0, 100.0);
		ImGui::SliderFloat("CutOff", &SpotLight->CutOff, 1.0, 100.0);
		ImGui::SliderFloat("OuterCutOff", &SpotLight->OuterCutOff, 1.0, 100.0);
		bool isEnable = SpotLight->isEnable;
		ImGui::Checkbox("is enable", &isEnable);
		SpotLight->isEnable = isEnable;
	}

	virtual std::string GetName() override
	{
		return "SpotLight";
	}
};

