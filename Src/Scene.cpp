#include "Scene.hpp"
#include <Foundation/Foundation.hpp>
#include "Util/Util.hpp"
#include "ImageIO/ImageIO.hpp"

FScene::FScene(FDiligentRenderer* Renderer, GLFWInputSystem* InputSystem)
	:Renderer(Renderer), InputSystem(InputSystem)
{
	LoadModels();
	CreateResources();
	PhongShadingPSHConstantsResource->PhongShadingPSHConstants.PointLight.Position = glm::vec3(0.0f, 0.0f, 1.5f);
	Camera = FCamera(Renderer->CurrentBackBufferSize().x, Renderer->CurrentBackBufferSize().y);
	Camera.Name = "Main camera";
}

FScene::~FScene()
{
}

void FScene::CreateResources()
{
	PhongShadingPSHConstantsResource = new FPhongShadingPSHConstantsResource(Renderer);

	StaticMeshPipeline = std::make_unique<FStaticMeshPipeline>(Renderer, PhongShadingPSHConstantsResource);
	SkeletonMeshPipeline = std::make_unique< FSkeletonMeshPipeline>(Renderer, PhongShadingPSHConstantsResource);
	SkyBoxPipeline = std::make_unique<FSkyBoxPipeline>(Renderer);
	DirectionLightDepthMapPipeline = std::make_unique<FDirectionLightDepthMapPipeline>(
		Renderer, 1024, 1024);
	Image2DPipeline = std::make_unique<FDepthMapImage2DPipeline>(Renderer);
	PBRPipeline = std::make_unique<FPBRPipeline>(Renderer);
}

void FScene::LoadModels()
{
	PBRStaticMesh = FStaticMesh::New(ks::Application::getResourcePath("Static/Suzanne.gltf"));
	PBRStaticMesh->Name = "PBRStaticMesh";
	PBRStaticMeshDrawable = new FPBRStaticMeshDrawable(Renderer, PBRStaticMesh);

	StaticMesh = FStaticMesh::New(ks::Application::getResourcePath("Static/Character.dae"));
	StaticMesh->Name = "StaticMesh";
	StaticMesh->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	StaticMesh->Rotation = glm::vec3(90.0f, 0.0f, 0.0f);
	StaticMeshDrawable = new FStaticMeshDrawable(Renderer, StaticMesh);

	FloorStaticMesh = FStaticMesh::New(ks::Application::getResourcePath("Floor/Floor.dae"));
	FloorStaticMesh->Name = "FloorStaticMesh";
	FloorStaticMesh->Position = glm::vec3(0.0, -1.0f, 0.0f);
	FloorStaticMesh->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	FloorStaticMeshDrawable = new FStaticMeshDrawable(Renderer, FloorStaticMesh);

	SkeletonMesh = FSkeletonMesh::New(ks::Application::getResourcePath("SkeletalAnimation/model.dae"));
	SkeletonMesh->Name = "SkeletonMesh";
	SkeletonMesh->Position = glm::vec3(0.0f, -1.0f, 0.0f);
	SkeletonMesh->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	SkeletonMesh->Rotation = glm::vec3(-90.0f, 0.0f, 180.0f);
	SkeletonMeshDrawable = new FSkeletonMeshDrawable(Renderer, SkeletonMesh);

	SkyBox = new FSkyBox({
		ks::Application::getResourcePath("Skybox/right.png"),
		ks::Application::getResourcePath("Skybox/left.png"),
		ks::Application::getResourcePath("Skybox/top.png"),
		ks::Application::getResourcePath("Skybox/bottom.png"),
		ks::Application::getResourcePath("Skybox/back.png"),
		ks::Application::getResourcePath("Skybox/front.png"),
	});
	SkyBoxDrawable = new FSkyBoxDrawable(Renderer, SkyBox);

	PointLightIndicatorMesh = FStaticMesh::New(ks::Application::getResourcePath("Light/cube.dae"));
	PointLightIndicatorMesh->Name = "PointLightIndicatorMesh";
	PointLightIndicatorMesh->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	PointLightIndicatorMeshDrawable =
		new FStaticMeshDrawable(Renderer, PointLightIndicatorMesh);
}

void FScene::Render(double RunningTime)
{
	RenderPBR(RunningTime);
	return;

	PhongShadingPSHConstantsResource->PhongShadingPSHConstants.SpotLight.Position =
		Camera.GetPosition();
	PhongShadingPSHConstantsResource->PhongShadingPSHConstants.SpotLight.Direction =
		Camera.GetCameraFront();
	PhongShadingPSHConstantsResource->PhongShadingPSHConstants.ViewPosition =
		Camera.GetPosition();
	PhongShadingPSHConstantsResource->Map();

	SkeletonMesh->RunningTime = RunningTime;
	SkeletonMesh->UpdateBoneTransform();
	PointLightIndicatorMesh->Position =
		PhongShadingPSHConstantsResource->PhongShadingPSHConstants.PointLight.Position;

	const std::vector<FStaticMeshDrawable*> ShadowStaticMeshDrawables = {
		StaticMeshDrawable,
	};

	const std::vector<FStaticMeshDrawable*> StaticMeshDrawables = {
		PointLightIndicatorMeshDrawable,
		StaticMeshDrawable,
		FloorStaticMeshDrawable,
	};

	const glm::mat4 LightProjection = DirectionLightShadowMapInfo.GetProjetionMat();
	const glm::mat4 LightView = DirectionLightShadowMapInfo.GetViewMat();
	DirectionLightDepthMapPipeline->DirectionLightDepthMapVSHConstants.LightSpaceMat =
		LightProjection * LightView;
	DirectionLightDepthMapPipeline->NewFrame();

	for (size_t i = 0; i < ShadowStaticMeshDrawables.size(); i++)
	{
		DirectionLightDepthMapPipeline->Render(RunningTime, ShadowStaticMeshDrawables[i]);
	}

	Renderer->SetRenderTarget();
	FStaticMeshPipeline::FRenderInfo RenderInfo;
	RenderInfo.RunningTime = RunningTime;
	RenderInfo.Camera = &Camera;
	StaticMeshPipeline->PhongShadingVSHConstants.LightSpaceMatrix =
		DirectionLightDepthMapPipeline->DirectionLightDepthMapVSHConstants.LightSpaceMat;
	for (size_t i = 0; i < StaticMeshDrawables.size(); i++)
	{
		RenderInfo.DirctionLightShadowMap = DirectionLightDepthMapPipeline->GetResourceView();
		RenderInfo.StaticMeshDrawable = StaticMeshDrawables[i];
		StaticMeshPipeline->Render(RenderInfo);
	}
	SkeletonMeshPipeline->Render(RunningTime, &Camera, SkeletonMeshDrawable);
	SkyBoxPipeline->Render(RunningTime, &Camera, SkyBoxDrawable);

	std::array<glm::vec2, 4> Quad;
	Quad[0] = glm::vec2(-1.0, 1.0);
	Quad[1] = glm::vec2(-0.5, 1.0);
	Quad[2] = glm::vec2(-1.0, 0.5);
	Quad[3] = glm::vec2(-0.5, 0.5);
	Image2DPipeline->Render(Quad, DirectionLightDepthMapPipeline->GetTexture());
}

void FScene::RenderPBR(double RunningTime)
{
	PBRPipeline->PBRVSHConstants.Projection = Camera.GetProjectionMat();
	PBRPipeline->PBRVSHConstants.View = Camera.GetViewMat();
	PBRPipeline->PBRVSHConstants.Model = PBRStaticMesh->GetModelMatrix();

	PBRPipeline->PBRPSHConstants.SpotLight.Position = Camera.GetPosition();
	PBRPipeline->PBRPSHConstants.SpotLight.Direction = Camera.GetCameraFront();
	PBRPipeline->PBRPSHConstants.ViewPosition = Camera.GetPosition();

	PBRPipeline->Render(&Camera, PBRStaticMeshDrawable);
	SkyBoxPipeline->Render(RunningTime, &Camera, SkyBoxDrawable);
}

void FScene::SaveBackBufferTexture()
{
	Diligent::ITexture* BackBufferTexture =
		Renderer->GetSwapChain()->GetCurrentBackBufferRTV()->GetTexture();
	const auto& SwapChainDesc = Renderer->GetSwapChain()->GetDesc();
	Diligent::RefCntAutoPtr<Diligent::ITexture> StagingTexture;
	Diligent::TextureDesc TexDesc;
	TexDesc.Name = "StagingTexture";
	TexDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
	TexDesc.Width = SwapChainDesc.Width;
	TexDesc.Height = SwapChainDesc.Height;
	TexDesc.Format = SwapChainDesc.ColorBufferFormat;
	TexDesc.Usage = Diligent::USAGE_STAGING;
	TexDesc.CPUAccessFlags = Diligent::CPU_ACCESS_READ;
	Renderer->GetDevice()->CreateTexture(TexDesc, nullptr, &StagingTexture);
	Diligent::CopyTextureAttribs CopyAttribs(BackBufferTexture,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
		StagingTexture,
		Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	Renderer->GetImmediateContext()->CopyTexture(CopyAttribs);
	const std::unique_ptr<ks::PixelBuffer> PixelBuffer =
		std::unique_ptr<ks::PixelBuffer>(Renderer->CreatePixelBuffer(StagingTexture));
	ImageIO::SaveImage(*PixelBuffer, "output.png");
}

void FScene::LoadUI(FMainView & MainView)
{
	MainView.PushComponent(&Camera);

	MainView.PushComponent(&PBRPipeline->PBRPSHConstants);
	MainView.PushComponent(StaticMesh);
	MainView.PushComponent(PBRStaticMesh);
	MainView.PushComponent(FloorStaticMesh);
	MainView.PushComponent(SkeletonMesh);
	MainView.PushComponent(PointLightIndicatorMesh);
	
	MainView.PushComponent(&PhongShadingPSHConstantsResource->PhongShadingPSHConstants.SpotLight);
	MainView.PushComponent(&PhongShadingPSHConstantsResource->PhongShadingPSHConstants.PointLight);
	MainView.PushComponent(&PhongShadingPSHConstantsResource->PhongShadingPSHConstants.DirectionalLight);

	MainView.PushComponent(&DirectionLightShadowMapInfo);
}

std::vector<IInput*> FScene::GetInput()
{
	std::vector<IInput*> Inputs;
	Inputs.push_back(&Camera);
	Inputs.push_back(this);
	return Inputs;
}

bool FScene::IsQuit() const
{
	return IsQuitFlag;
}

void FScene::NotifyKeyInput(Keyboard::KeyType KeyType, Keyboard::ActionType ActionType, Keyboard::ModeFlag ModeFlag)
{
	if (KeyType == Keyboard::KeyType::KEY_ESCAPE && ActionType == Keyboard::ActionType::Press)
	{
		IsQuitFlag = true;
	}
	if (KeyType == Keyboard::KeyType::KEY_F && ActionType == Keyboard::ActionType::Press)
	{
		PhongShadingPSHConstantsResource->PhongShadingPSHConstants.SpotLight.isEnable =
			!PhongShadingPSHConstantsResource->PhongShadingPSHConstants.SpotLight.isEnable;
	}
	if (KeyType == Keyboard::KeyType::KEY_F1 && ActionType == Keyboard::ActionType::Press)
	{
		Keyboard::CursorMode Mode = InputSystem->GetCursorMode();
		if (Mode == Keyboard::CursorMode::Disabled || Mode == Keyboard::CursorMode::Hidden)
		{
			InputSystem->SetCursorMode(Keyboard::CursorMode::Normal);
			Camera.IsMouseMovable = false;
		}
		else
		{
			InputSystem->SetCursorMode(Keyboard::CursorMode::Disabled);
			Camera.IsMouseMovable = true;
		}
	}
	if (KeyType == Keyboard::KeyType::KEY_M && ActionType == Keyboard::ActionType::Press)
	{
		SaveBackBufferTexture();
	}
}

void FScene::NotifyKeyActionChange(std::function<Keyboard::ActionType(Keyboard::KeyType)> select)
{
}

void FScene::NotifyMousePositionChange(double Xpos, double Ypos)
{
}

void FScene::NotifyMouseScroll(double Xoffset, double Yoffset)
{
}
