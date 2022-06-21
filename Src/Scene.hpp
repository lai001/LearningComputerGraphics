#pragma once
#include <vector>
#include "Interface/GLRunLoop.hpp"
#include "Interface/Input.hpp"
#include "GUI/Main/MainView.hpp"
#include "GUI/GLFWInputSystem.hpp"
#include "Renderer/StaticMeshPipeline.hpp"
#include "Renderer/SkeletonMeshPipeline.hpp"
#include "Renderer/SkyBoxPipeline.hpp"
#include "Renderer/DirectionLightDepthMapPipeline.hpp" 
#include "Renderer/DepthMapImage2DPipeline.hpp"

class FScene : public IRenderRunLoop, public IInput
{
private:
	FDiligentRenderer* Renderer = nullptr;

	FPhongShadingPSHConstantsResource* PhongShadingPSHConstantsResource = nullptr;

	std::unique_ptr<FStaticMeshPipeline> StaticMeshPipeline = nullptr;
	std::unique_ptr<FSkeletonMeshPipeline> SkeletonMeshPipeline = nullptr;
	std::unique_ptr<FSkyBoxPipeline> SkyBoxPipeline = nullptr;
	std::unique_ptr<FDirectionLightDepthMapPipeline> DirectionLightDepthMapPipeline = nullptr;
	std::unique_ptr<FDepthMapImage2DPipeline> Image2DPipeline = nullptr;

	FStaticMesh* StaticMesh = nullptr;
	FStaticMeshDrawable* StaticMeshDrawable = nullptr;

	FStaticMesh* FloorStaticMesh = nullptr;
	FStaticMeshDrawable* FloorStaticMeshDrawable = nullptr;

	FSkeletonMesh* SkeletonMesh = nullptr;
	FSkeletonMeshDrawable* SkeletonMeshDrawable = nullptr;

	FSkyBox* SkyBox = nullptr;
	FSkyBoxDrawable* SkyBoxDrawable = nullptr;

	FStaticMesh* PointLightIndicatorMesh = nullptr;
	FStaticMeshDrawable* PointLightIndicatorMeshDrawable = nullptr;

	FDirectionLightShadowMapInfo DirectionLightShadowMapInfo;

	FCamera Camera;

	bool IsQuitFlag = false;

	GLFWInputSystem* InputSystem = nullptr;;

	void CreateResources();

	void LoadModels();

public:
	FScene(FDiligentRenderer* Renderer, GLFWInputSystem* InputSystem);
	~FScene();

	virtual void Render(double RunningTime) override;

	void LoadUI(FMainView& MainView);

	std::vector<IInput*> GetInput();

	bool IsQuit() const;

	virtual void NotifyKeyInput(Keyboard::KeyType KeyType, Keyboard::ActionType ActionType, Keyboard::ModeFlag ModeFlag) override;
	virtual void NotifyKeyActionChange(std::function<Keyboard::ActionType(Keyboard::KeyType)> select) override;
	virtual void NotifyMousePositionChange(double Xpos, double Ypos) override;
	virtual void NotifyMouseScroll(double Xoffset, double Yoffset) override;
};
