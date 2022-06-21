#pragma once
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "Interface/GUIInterface.hpp"
#include "Interface/GUISystem.hpp"

#include "CameraUIComponent.hpp"
#include "DirectionalLightUIComponent.hpp"
#include "PointLightUIComponent.hpp"
#include "SkeletonMeshUIComponent.hpp"
#include "SkyBoxUIComponent.hpp"
#include "SpotLightUIComponent.hpp"
#include "StaticMeshUIComponent.hpp"
#include "DirectionLightDepthMap.hpp"

class FMainView : public IGUIInterface
{
public:
	FMainView();
	~FMainView();

	virtual void Render(double RunningTime) override;

public:
	std::vector<IObjectPropertyUIComponent*> ObjectPropertyUIComponents;
	IObjectPropertyUIComponent* SelectedObjectPropertyUIComponent = nullptr;

	bool bIsShowObjectPropertyWindow = false;
	bool bIsHidden = false;

	void PushComponent(FStaticMesh* Mesh)
	{
		FStaticMeshUIComponent* Component = new FStaticMeshUIComponent();
		Component->Mesh = Mesh;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FSkeletonMesh* Mesh)
	{
		FSkeletonMeshUIComponent* Component = new FSkeletonMeshUIComponent();
		Component->Mesh = Mesh;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FDirectionalLight* DirectionalLight)
	{
		FDirectionalLightUIComponent* Component = new FDirectionalLightUIComponent();
		Component->DirectionalLight = DirectionalLight;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FCamera* Camera)
	{
		FCameraUIComponent* Component = new FCameraUIComponent();
		Component->Camera = Camera;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FSkyBox* SkyBox)
	{
		FSkyBoxUIComponent* Component = new FSkyBoxUIComponent();
		Component->SkyBox = SkyBox;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FPointLight* PointLight)
	{
		FPointLightUIComponent* Component = new FPointLightUIComponent();
		Component->PointLight = PointLight;
		ObjectPropertyUIComponents.push_back(Component);
	}

	void PushComponent(FSpotLight* SpotLight)
	{
		FSpotLightUIComponent* Component = new FSpotLightUIComponent();
		Component->SpotLight = SpotLight;
		ObjectPropertyUIComponents.push_back(Component);
	}	

	void PushComponent(FDirectionLightShadowMapInfo* DirectionLightShadowMapInfo)
	{
		FDirectionLightDepthMapUIComponent* Component = new FDirectionLightDepthMapUIComponent();
		Component->DirectionLightShadowMapInfo = DirectionLightShadowMapInfo;
		ObjectPropertyUIComponents.push_back(Component);
	}

private:
	void Tree(const std::string& Name, const std::function<void()>& Closure);
	void RenderWindow(const std::string & Name, bool * bIsOpen, const ImGuiWindowFlags flags, std::function<void()> Closure);
	void RenderChild(const std::string & ID, const ImVec2 & Size, bool Border, ImGuiWindowFlags ExtraFlags, std::function<void()> Closure);
};