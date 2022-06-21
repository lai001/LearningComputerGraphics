#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "Interface/ObjectPropertyUIComponent.hpp"
#include "Object/Camera.hpp"

struct FCameraUIComponent : public IObjectPropertyUIComponent
{
	FCamera* Camera = nullptr;

	virtual void Draw() override
	{
		assert(Camera);
		glm::vec3 Position = Camera->GetPosition();
		float CameraSpeed = Camera->GetCameraSpeed();
		float CameraFov = Camera->GetFov();

		ImGui::Text(GetName().c_str());
		ImGui::DragFloat("camera fov", &CameraFov, 0.05f, 1.0, 100.0f);
		ImGui::DragFloat("camera speed", &CameraSpeed, 0.02f, 0.01f, 10.0);
		ImGui::DragFloat3("camera postion", glm::value_ptr(Position), 0.05f , -100.0f, 100.0f);
		ImGui::Checkbox("camera is mouse movable", &Camera->IsMouseMovable);
		ImGui::Checkbox("camera is movable", &Camera->IsMovable);

		Camera->SetCameraSpeed(CameraSpeed);
		Camera->SetFov(CameraFov);
		Camera->SetPosition(Position);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Camera, Camera->Name);
	}
};