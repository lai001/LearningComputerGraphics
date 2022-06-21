#pragma once
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <DiligentTools/Imgui/interface/ImGuiDiligentRenderer.hpp>
#include <DiligentTools/Imgui/interface/ImGuiImplWin32.hpp>
#include "Renderer/DiligentRenderer.hpp"
#include "Interface/GUIInterface.hpp"
#include "Interface/GUIRenderer.hpp"

class FDiligentGUIRenderer: public IGUIRenderer
{
public:
	FDiligentGUIRenderer(FDiligentRenderer* Renderer, GLFWwindow* glfwWindow);
	~FDiligentGUIRenderer();

	virtual void Render(double RunningTime) override;

	Diligent::ImGuiImplWin32* m_pImGui = nullptr;

	FDiligentRenderer* Renderer = nullptr;

	IGUIInterface* GUIInterface = nullptr;

	GLFWwindow* glfwWindow = nullptr;

	void NewFrame();

	virtual void WindowSizeChange(const int width, const int height) override;
};
