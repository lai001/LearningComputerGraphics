#pragma once
#include "ThirdParty/spdlog.h"

#include "GUIInterface.h"
#include "GUISystem.h"
#include "LightingSystem/LightingSystem.h"
#include "StaticMesh.h"
#include "Camera.h"

class IObjectPropertyUIComponent
{
public:
	virtual std::string GetName() = 0;
	virtual void Draw() = 0;
};

struct FStaticMeshUIComponent : public IObjectPropertyUIComponent
{
	FStaticMesh* Mesh = nullptr;

	virtual void Draw() override
	{
		assert(Mesh);
		ImGui::Text(GetName().c_str());
		ImGui::SliderFloat3("position", glm::value_ptr(Mesh->Position), -10.0f, 10.0f);
		ImGui::SliderFloat3("scale", glm::value_ptr(Mesh->Scale), -10.0f, 10.0f);
		ImGui::SliderFloat3("rotation", glm::value_ptr(Mesh->Rotation), -360.0f, 360.0f);
		ImGui::Checkbox("is visible", &Mesh->bIsVisible);
		ImGui::Checkbox("is unlit", &Mesh->bIsUnlit);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Mesh, Mesh->Name);
	}
};

struct FSkeletonMeshUIComponent : public IObjectPropertyUIComponent
{
	FSkeletonMesh* Mesh = nullptr;

	virtual void Draw() override
	{
		assert(Mesh);
		ImGui::Text(GetName().c_str());
		ImGui::SliderFloat3("position", glm::value_ptr(Mesh->Position), -10.0f, 10.0f);
		ImGui::SliderFloat3("scale", glm::value_ptr(Mesh->Scale), -10.0f, 10.0f);
		ImGui::SliderFloat3("rotation", glm::value_ptr(Mesh->Rotation), -360.0f, 360.0f);
		ImGui::Checkbox("is visible", &Mesh->bIsVisible);
		ImGui::Checkbox("is unlit", &Mesh->bIsUnlit);
		ImGui::SliderFloat("Animation speed", &Mesh->Speed, 0.0f, 10.0f);
		ImGui::Checkbox("Binding post", &Mesh->bIsEnableBindingPost);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Mesh, Mesh->Name);
	}
};

struct FDirectionalLightUIComponent : public IObjectPropertyUIComponent
{
	FDirectionalLight* DirectionalLight = nullptr;
	FStaticMesh* Mesh = nullptr;

	virtual void Draw() override
	{
		assert(DirectionalLight);
		assert(Mesh);
		ImGui::Text(GetName().c_str());
		ImGui::SliderFloat3("position", glm::value_ptr(Mesh->Position), -10.0f, 10.0f);
		ImGui::SliderFloat3("scale", glm::value_ptr(Mesh->Scale), -10.0f, 10.0f);
		ImGui::SliderFloat3("rotation", glm::value_ptr(DirectionalLight->Direction), -360.0f, 360.0f);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(DirectionalLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(DirectionalLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(DirectionalLight->Specular), -100.0, 100.0);
		ImGui::Checkbox("is visible", &Mesh->bIsVisible);
		ImGui::Checkbox("is enable", &DirectionalLight->bIsDirectionalLightEnable);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Mesh, Mesh->Name);
	}
};

struct FCameraUIComponent : public IObjectPropertyUIComponent
{
	FCamera* Camera = nullptr;

	virtual void Draw() override
	{
		assert(Camera);
		glm::vec3 Position = Camera->GetPosition();

		ImGui::Text(GetName().c_str());
		ImGui::Text("camera speed %.3f", Camera->GetCameraSpeed());
		ImGui::Text("camera fov %.3f", Camera->GetFov());
		ImGui::Text("camera postion x: %.3f, y: %.3f, z: %.3f", Position.x, Position.y, Position.z);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)Camera, Camera->Name);
	}
};

struct FSkyBoxUIComponent : public IObjectPropertyUIComponent
{
	FSkyBox* SkyBox = nullptr;

	virtual void Draw() override
	{
		assert(SkyBox);
		ImGui::Text(GetName().c_str());
		ImGui::Checkbox("is visible", &SkyBox->bIsVisible);
	}

	virtual std::string GetName() override
	{
		return fmt::format("{} - {}", (void*)SkyBox, SkyBox->Name);
	}
};

struct FShadowUIComponent : public IObjectPropertyUIComponent
{
	FShadow* Shadow = nullptr;

	virtual void Draw() override
	{
		assert(Shadow);
		ImGui::SliderFloat4("LightProjectionView", glm::value_ptr(Shadow->LightProjectionView), -100.0, 100.0);
		ImGui::SliderFloat("ProjectionNear", &Shadow->ProjectionNear, 0.01, 1000.0);
		ImGui::SliderFloat("ProjectionFar", &Shadow->ProjectionFar, 0.01 * 2.0, 10000.0);
		ImGui::SliderFloat4("LightProjectionView", glm::value_ptr(Shadow->LightProjectionView), -100.0, 100.0);
		ImGui::SliderFloat3("LightViewEye", glm::value_ptr(Shadow->LightViewEye), -100.0, 100.0);
		ImGui::SliderFloat3("LightViewCenter", glm::value_ptr(Shadow->LightViewCenter), -100.0, 100.0);
		ImGui::SliderFloat3("LightViewUp", glm::value_ptr(Shadow->LightViewUp), -100.0, 100.0);
		ImGui::Checkbox("is enable", &Shadow->bIsShadowEnable);
	}

	virtual std::string GetName() override
	{
		return "Shadow";
	}
};

struct FPointLightUIComponent : public IObjectPropertyUIComponent
{
	FPointLight* PointLight = nullptr;

	virtual void Draw() override
	{
		assert(PointLight);
		ImGui::SliderFloat3("Position", glm::value_ptr(PointLight->Position), -100.0, 100.0);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(PointLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(PointLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(PointLight->Specular), -100.0, 100.0);
		ImGui::SliderFloat("Constant", &PointLight->Constant, 1.0, 100.0);
		ImGui::SliderFloat("Linear", &PointLight->Linear, 1.0, 100.0);
		ImGui::SliderFloat("Quadratic", &PointLight->Quadratic, 1.0, 100.0);
		ImGui::Checkbox("is enable", &PointLight->bIsPointLightEnable);
	}

	virtual std::string GetName() override
	{
		return "PointLight";
	}
};

struct FSpotLightUIComponent : public IObjectPropertyUIComponent
{
	FSpotLight* SpotLight = nullptr;

	virtual void Draw() override
	{
		assert(SpotLight);
		ImGui::SliderFloat3("Position", glm::value_ptr(SpotLight->Position), -100.0, 100.0);
		ImGui::SliderFloat3("Direction", glm::value_ptr(SpotLight->Direction), -100.0, 100.0);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(SpotLight->Ambient), -100.0, 100.0);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(SpotLight->Diffuse), -100.0, 100.0);
		ImGui::SliderFloat3("Specular", glm::value_ptr(SpotLight->Specular), -100.0, 100.0);
		ImGui::SliderFloat("Constant", &SpotLight->Constant, 1.0, 100.0);
		ImGui::SliderFloat("Linear", &SpotLight->Linear, 1.0, 100.0);
		ImGui::SliderFloat("Quadratic", &SpotLight->Quadratic, 1.0, 100.0);
		ImGui::SliderFloat("CutOff", &SpotLight->CutOff, 1.0, 100.0);
		ImGui::SliderFloat("OuterCutOff", &SpotLight->OuterCutOff, 1.0, 100.0);
		ImGui::Checkbox("is enable", &SpotLight->bIsSpotLightEnable);
	}

	virtual std::string GetName() override
	{
		return "SpotLight";
	}
};

class FMainView : public IGUIInterface
{
public:
	FMainView();
	~FMainView();

	virtual void Render(float RunningTime) override;

public:
	int PostProcessing = 0;
	bool bIsFrameBufferEnable = false;
	bool bIsShowDepthMap = false;

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

	void PushComponent(FDirectionalLight* DirectionalLight, FStaticMesh* Mesh)
	{
		FDirectionalLightUIComponent* Component = new FDirectionalLightUIComponent();
		Component->Mesh = Mesh;
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

	void PushComponent(FShadow* Shadow)
	{
		FShadowUIComponent* Component = new FShadowUIComponent();
		Component->Shadow = Shadow;
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

private:
	void Tree(const std::string& Name, const std::function<void()>& Closure);
	void RenderWindow(const std::string & Name, bool * bIsOpen, const ImGuiWindowFlags flags, std::function<void()> Closure);
	void RenderChild(const std::string & ID, const ImVec2 & Size, bool Border, ImGuiWindowFlags ExtraFlags, std::function<void()> Closure);
};