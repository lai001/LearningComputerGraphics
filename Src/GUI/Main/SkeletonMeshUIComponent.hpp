#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "Object/SkeletonMesh.hpp"

struct FSkeletonMeshUIComponent : public IObjectPropertyUIComponent
{
	FSkeletonMesh* Mesh = nullptr;

	virtual void Draw() override
	{
		assert(Mesh);
		ImGui::Text(GetName().c_str());
		ImGui::DragFloat3("position", glm::value_ptr(Mesh->Position), 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("scale", glm::value_ptr(Mesh->Scale), 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("rotation", glm::value_ptr(Mesh->Rotation), 0.5f, -360.0f, 360.0f);
		ImGui::Checkbox("is visible", &Mesh->bIsVisible);
		//ImGui::Checkbox("is unlit", &Mesh->bIsUnlit);
		ImGui::DragFloat("Animation speed", &Mesh->Speed, 0.1f, 0.0f, 1000.0f);
		ImGui::Checkbox("Binding post", &Mesh->bIsEnableBindingPost);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Mesh, Mesh->Name);
	}
};
