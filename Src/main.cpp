//#include "App.h"

#include <Foundation/Foundation.hpp>
#include <spdlog/spdlog.h>
#include "Renderer/DiligentRenderer.hpp"
#include "GUI/GLFWGUISystem.hpp"
#include "GUI/Main/MainView.hpp"
#include "GUI/DiligentGUIRenderer.hpp"
#include "Scene.hpp"

int main(int arg, char** args)
{
	ks::Application::Init(arg, args);
	spdlog::set_level(spdlog::level::trace);
	//FApp App(arg, args);

	GLFWGUISystem::FCreateInfo info;

	GLFWGUISystem* GuiSystem = new GLFWGUISystem(info);
	FDiligentRenderer* DiligentRenderer = new FDiligentRenderer(GuiSystem->GetWin32Window());
	FDiligentGUIRenderer* DiligentGUIRenderer = new FDiligentGUIRenderer(DiligentRenderer, GuiSystem->GetGlfwWindow());
	FMainView* MainView = new FMainView();
	DiligentGUIRenderer->GUIInterface = MainView;
	GuiSystem->GUIRenderer = DiligentGUIRenderer;

	FScene* Scene = new FScene(DiligentRenderer, GuiSystem->GetGLFWInputSystem());
	Scene->LoadUI(*MainView);
	GuiSystem->GetGLFWInputSystem()->AddInputs(Scene->GetInput());
	while (GuiSystem->ShouldClose() == false)
	{
		DiligentRenderer->SetRenderTarget();
		DiligentGUIRenderer->NewFrame();
		Scene->Render(GuiSystem->CurrentTime());
		GuiSystem->Render(GuiSystem->CurrentTime());
		if (Scene->IsQuit())
		{
			GuiSystem->Close();
		}
	}
	return 0;
}