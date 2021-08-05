#include "GUISystem.h"

FGUISystem::FGUISystem(GLFWwindow* Window)
	:Window(Window)
{
	Init();
}

FGUISystem::~FGUISystem()
{
	Destroy();
}

const GLFWwindow * FGUISystem::GetWindow()
{
	return this->Window;
}

void FGUISystem::Init()
{
	ImGui::CreateContext();
	ImGuiIO& Io = ImGui::GetIO();
	(void)Io;
	ImGui_ImplGlfwGL3_Init(this->Window, false);
}

void FGUISystem::Tick(GLFWwindow * Window, double RunningTime)
{
	ImGui_ImplGlfwGL3_NewFrame();
	if (Interface)
	{
		Interface->Render(RunningTime);
	}
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void FGUISystem::Destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}