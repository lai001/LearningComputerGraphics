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
	const std::string Version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	assert(ImGui_ImplOpenGL3_Init(Version.c_str()));
}

void FGUISystem::Tick(const GLFWwindow& Window, double RunningTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (Interface)
	{
		Interface->Render(RunningTime);
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FGUISystem::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}