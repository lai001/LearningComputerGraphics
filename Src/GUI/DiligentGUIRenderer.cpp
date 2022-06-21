#include "DiligentGUIRenderer.hpp"
#include <backends/imgui_impl_glfw.h>
#include <Foundation/Foundation.hpp>

FDiligentGUIRenderer::FDiligentGUIRenderer(FDiligentRenderer* Renderer, GLFWwindow* glfwWindow)
	:Renderer(Renderer), glfwWindow(glfwWindow)
{
	const Diligent::SwapChainDesc &SCDesc = Renderer->GetSwapChain()->GetDesc();
	m_pImGui = new Diligent::ImGuiImplWin32(Renderer->GetHWND(), Renderer->GetDevice(), SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat);
	ImGui_ImplGlfw_InitForOther(glfwWindow, true);
}

FDiligentGUIRenderer::~FDiligentGUIRenderer()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void FDiligentGUIRenderer::Render(double RunningTime)
{
	if (GUIInterface)
	{
		ImGui_ImplGlfw_NewFrame();
		const auto& SCDesc = Renderer->GetSwapChain()->GetDesc();
		m_pImGui->NewFrame(SCDesc.Width, SCDesc.Height, SCDesc.PreTransform);
		Renderer->SetRenderTarget();
		GUIInterface->Render(RunningTime);
		m_pImGui->Render(Renderer->GetImmediateContext());
		Renderer->Present(1);
	}
}

void FDiligentGUIRenderer::NewFrame()
{
	Renderer->ClearColor();
	Renderer->ClearDepth();
	Renderer->ClearStencil();
}

void FDiligentGUIRenderer::WindowSizeChange(const int width, const int height)
{
	Renderer->GetSwapChain()->Resize(width, height);
}