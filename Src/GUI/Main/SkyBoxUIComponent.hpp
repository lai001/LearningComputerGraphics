#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "Object/SkyBox.hpp"

struct FSkyBoxUIComponent : public IObjectPropertyUIComponent
{
	FSkyBox* SkyBox = nullptr;

	virtual void Draw() override
	{
		assert(SkyBox);
		ImGui::Text(GetName().c_str());
		ImGui::Checkbox("is visible", &SkyBox->bIsVisible);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)SkyBox, SkyBox->Name);
	}
};
