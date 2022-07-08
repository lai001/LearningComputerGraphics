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
		ImGui::Text("CameraFront x = %.2f, y = %.2f, z = %.2f", Camera->GetCameraFront().x, Camera->GetCameraFront().y, Camera->GetCameraFront().z);
		ImGui::DragFloat("Camera fov", &CameraFov, 0.05f, 1.0, 100.0f);
		ImGui::DragFloat("Camera speed", &CameraSpeed, 0.02f, 0.01f, 10.0);
		ImGui::DragFloat3("Camera postion", glm::value_ptr(Position), 0.05f , -100.0f, 100.0f);
		ImGui::Checkbox("Camera is mouse movable", &Camera->IsMouseMovable);
		ImGui::Checkbox("Camera is movable", &Camera->IsMovable);

		Camera->SetCameraSpeed(CameraSpeed);
		Camera->SetFov(CameraFov);
		Camera->SetPosition(Position);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Camera, Camera->Name);
	}
};