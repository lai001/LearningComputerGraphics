#include "MainView.hpp"

FMainView::FMainView()
{

}

FMainView::~FMainView()
{
	for (auto UIComponent : ObjectPropertyUIComponents)
	{
		delete UIComponent;
	}
}

void FMainView::Render(double RunningTime)
{
	if (bIsHidden)
	{
		return;
	}
	ImGuiWindowFlags Flags = 0;
	Flags |= ImGuiWindowFlags_MenuBar;
	//ImGui::SetNextWindowPos(ImVec2(60, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(500, 500), nullptr, nullptr);
	RenderWindow("Debug", nullptr, Flags, [&]()
	{
		ImGui::Text("Time %.3f, Application average %.3f ms/frame (%.1f FPS)", RunningTime, 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("Objects"))
		{
			ImGui::Checkbox("ShowObjectPropertyWindow", &bIsShowObjectPropertyWindow);

			RenderChild("Objects", ImVec2(300, 0), true, 0, [&]()
			{
				for (auto ObjectPropertyUIComponent : ObjectPropertyUIComponents)
				{
					const std::string Name = ObjectPropertyUIComponent->GetName();
					if (ImGui::Selectable(Name.c_str(), ObjectPropertyUIComponent == SelectedObjectPropertyUIComponent))
					{
						SelectedObjectPropertyUIComponent = ObjectPropertyUIComponent;
						bIsShowObjectPropertyWindow = true;
					}
				}
			});
		}
	});

	if (bIsShowObjectPropertyWindow && SelectedObjectPropertyUIComponent)
	{
		//ImGui::SetNextWindowPos(ImVec2(60, 320));
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(600, 500), nullptr, nullptr);
		RenderWindow("ObjectProperty", nullptr, 0, [&]()
		{
			SelectedObjectPropertyUIComponent->Draw();
		});
	}
}

void FMainView::Tree(const std::string & Name, const std::function<void()>& Closure)
{
	if (ImGui::TreeNode(Name.c_str()))
	{
		Closure();
		ImGui::TreePop();
	};
}

void FMainView::RenderWindow(const std::string& Name, bool* bIsOpen, const ImGuiWindowFlags flags, std::function<void()> Closure)
{
	ImGui::SetNextWindowBgAlpha(0.2);
	if (ImGui::Begin(Name.c_str(), bIsOpen, flags))
	{
		Closure();
	}
	ImGui::End();
}

void FMainView::RenderChild(const std::string& ID, const ImVec2& Size, bool Border, ImGuiWindowFlags ExtraFlags, std::function<void()> Closure)
{
	if (ImGui::BeginChild(ID.c_str(), Size, Border, ExtraFlags))
	{
		Closure();
	}
	ImGui::EndChild();
}