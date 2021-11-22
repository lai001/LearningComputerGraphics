#include "NutGUI.h"
#include "ThirdParty/imgui.h"
#include "LightingSystem.h"

FNutGUI::FNutGUI()
{
	
}

FNutGUI::~FNutGUI()
{
}

void FNutGUI::Render(float RunningTime)
{
	{
		bool ToolActive = true;
		auto CWindow = ImGui::GetCurrentWindow();
		CWindow->Flags = CWindow->Flags | ImGuiWindowFlags_MenuBar;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) 
				{ 
					/* Do stuff */
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) 
				{
					/* Do stuff */
				}
				if (ImGui::MenuItem("Close", "Ctrl+W")) 
				{ 
					ToolActive = false; 
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	ImGui::Text("");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Current camera speed %.3f", CurrentCameraSpeed);
	ImGui::Text("Current fov %.3f", CurrentFOV);
	ImGui::Text("Cut off %.3f", glm::cos(glm::radians(10.5f)));
	ImGui::Text("Cut out off %.3f", glm::cos(glm::radians(12.5f)));
	ImGui::Text("Time %.3f", RunningTime);
	ImGui::SliderFloat3("Light position", &PointLightPosition.x, -10.0f, 10.0f);
	ImGui::SliderInt("PostProcessing", &PostProcessing, 0, 5);
	ImGui::SliderFloat("Animation speed", &ModelAnimationSpeed, 0.0f, 10.0f);
	ImGui::Checkbox("Binding post", &BindingPostEnable);
	ImGui::Checkbox("FrameBuffer", &bIsFrameBufferEnable);
	ImGui::Checkbox("Skybox ", &bIsShowSkybox);
}
