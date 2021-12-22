#include "App.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "ThirdParty/spdlog.h"
#include "ThirdParty/stb_image.h"
#include "ThirdParty/glm.h"

FApp::FApp(int arg, char** args)
{
	FileManager = new FFileManager(arg, args);
	spdlog::set_level(spdlog::level::trace);
	spdlog::debug(FileManager->GetResouceDir());
	FGLShader::SetShadersFolder(FileManager->GetResourcePath("GLShaders"));
	GLRenderer = new FGLRenderer(1440, 900, "LearningComputerGraphics");
	GLRenderer->MakeContextCurrent();

	InputCenter = GLRenderer->GetInputCenter();
	LightingSystem = new FLightingSystem();
	GUISystem = new FGUISystem(GLRenderer->GetWindow());
	MainView = new FMainView();
	GUISystem->Interface = MainView;

	Camera = new FCamera(GLRenderer->GetScreenWidth(), GLRenderer->GetScreenHeight());
	Camera->Name = "Camera";

	InputCenter->AddInput(this);
	InputCenter->AddInput(Camera);

	GLRenderer->AddRunLoop(this);
	GLRenderer->AddRunLoop(GUISystem);

	GLInfo Info = GLRenderer->GetGLInfo();
	spdlog::debug("{}, {}, {}", Info.Version, Info.Vendor, Info.Renderer);

	{
		LightingSystem->Shadow.LightProjectionView = glm::vec4(-10.0f, 10.0f, -10.0f, 10.0f);
		LightingSystem->Shadow.ProjectionFar = 7.5;
		LightingSystem->Shadow.ProjectionNear = 1.0;

		LightingSystem->Shadow.LightViewEye = glm::vec3(-2.0f, 4.0f, -1.0f);
		LightingSystem->Shadow.LightViewCenter = glm::vec3(0.0f, 0.0f, 0.0f);
		LightingSystem->Shadow.LightViewUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	{
		DepthMapFrameBuffer = new FGLDepthMapFrameBuffer(GLRenderer->GetScreenWidth(),
			GLRenderer->GetScreenHeight());

		DepthMapFrameBufferDebugDrawable = new FGLDepthMapFrameBufferDebugDrawable(*DepthMapFrameBuffer,
			glm::vec2(-1.0 + 2.0 / 3.0, 1.0),
			glm::vec2(2.0 / 3.0, 2.0 / 3.0));
	}

	{
		Mesh = FStaticMesh::New(FileManager->GetResourcePath("Character.dae"));
		Mesh->Name = "Character";
		Mesh->Position = glm::vec3(0.0, 1.0, 0.0);
		Mesh->Rotation = glm::vec3(0.0);
		Mesh->Scale = glm::vec3(0.2);
		Mesh->View = Camera->GetViewMat();
		Mesh->Projection = Camera->GetprojectionMat();
		Mesh->LightingSystem = LightingSystem;
		Drawable = new FGLStaticMeshDrawable(Mesh);
	}

	{
		FloorMesh = FStaticMesh::New(FileManager->GetResourcePath("Floor.dae"));
		FloorMesh->Name = "Floor";

		FloorMesh->Position = glm::vec3(0.0, -2.0, 0.0);
		FloorMesh->Rotation = glm::vec3(-90.0, 0.0, 0.0);
		FloorMesh->Scale = glm::vec3(1.0);

		FloorMesh->View = Camera->GetViewMat();
		FloorMesh->Projection = Camera->GetprojectionMat();
		FloorMesh->LightingSystem = LightingSystem;
		FloorMeshDrawable = new FGLStaticMeshDrawable(FloorMesh);
	}

	{
		std::vector<std::string> SkyBoxFilePaths{
			FileManager->GetResourcePath("skybox/right.png"),
			FileManager->GetResourcePath("skybox/left.png"),
			FileManager->GetResourcePath("skybox/bottom.png"),
			FileManager->GetResourcePath("skybox/top.png"),
			FileManager->GetResourcePath("skybox/front.png"),
			FileManager->GetResourcePath("skybox/back.png")
		};

		Skybox = new FSkyBox(SkyBoxFilePaths);
		Skybox->Name = "Skybox";
		SkyboxDrawable = new FGLSkyboxDrawable(Skybox);
	}

	{
		FrameBuffer = new FGLFrameBuffer(GLRenderer->GetScreenWidth(),
			GLRenderer->GetScreenHeight(),
			glm::vec2(-1.0, 1.0),
			glm::vec2(2.0 / 3.0, 2.0 / 3.0));
		FrameBuffer->ClearColor = glm::vec4(0.3f, 0.4f, 0.5f, 1.0f);
		FrameBuffer->ClearBufferFlags = EClearBufferFlags::Color | EClearBufferFlags::Depth;
	}

	{
		SkeletonMesh = new FSkeletonMesh(FileManager->GetResourcePath("skeletal_animation/model.dae"));
		SkeletonMesh->Name = "skeletal_animation";
		SkeletonMesh->Position = glm::vec3(0.0f, -0.9f, 0.0f);
		SkeletonMesh->Rotation = glm::vec3(-90.0, 0.0, 0.0);
		SkeletonMesh->Scale = glm::vec3(0.2f, 0.2f, 0.2f);

		SkeletonMesh->View = Camera->GetViewMat();
		SkeletonMesh->Projection = Camera->GetprojectionMat();
		SkeletonMesh->Speed = 1.0;
		SkeletonMesh->LightingSystem = LightingSystem;

		SkeletonMeshDrawable = new FGLSkeletonMeshDrawable(SkeletonMesh);
	}

	{
		DirectionalLightMesh = FStaticMesh::New(FileManager->GetResourcePath("light/cube.dae"));
		DirectionalLightMesh->Name = "DirectionalLight";
		DirectionalLightMesh->bIsUnlit = true;

		DirectionalLightMesh->Position = glm::vec3(0.0);
		DirectionalLightMesh->Rotation = glm::vec3(0.0);
		DirectionalLightMesh->Scale = glm::vec3(0.1);

		DirectionalLightMesh->View = Camera->GetViewMat();
		DirectionalLightMesh->Projection = Camera->GetprojectionMat();
		DirectionalLightMesh->LightingSystem = LightingSystem;
		DirectionalLightMeshDrawable = new FGLStaticMeshDrawable(DirectionalLightMesh);
	}

	std::vector<FStaticMesh*> StaticMeshs = { Mesh, FloorMesh };
	for (auto Mesh : StaticMeshs)
	{
		MainView->PushComponent(Mesh);
	}
	MainView->PushComponent(SkeletonMesh);
	MainView->PushComponent(Camera);
	MainView->PushComponent(Skybox);
	MainView->PushComponent(&LightingSystem->DirLight, DirectionalLightMesh);
	MainView->PushComponent(&LightingSystem->Shadow);
	MainView->PushComponent(&LightingSystem->SpotLight);
	MainView->PushComponent(&LightingSystem->PointLight);

	GLRenderer->StartMainLoop();
}

FApp::~FApp()
{
	delete GLRenderer;
	delete LightingSystem;
	delete GUISystem;
	delete MainView;
	delete Camera;
	delete Mesh;
	delete Drawable;
	delete FrameBuffer;
	delete DepthMapFrameBuffer;
	delete DepthMapFrameBufferDebugDrawable;
	delete FileManager;
}

void FApp::UpdateProperty(const FMainView & MainView)
{

}

void FApp::Tick(const GLFWwindow& Window, double RunningTime)
{
	LightingSystem->SpotLight.Position = Camera->GetPosition();
	LightingSystem->SpotLight.Direction = Camera->GetCameraFront();
	LightingSystem->ViewPosition = Camera->GetPosition();

	DirectionalLightMesh->Rotation = LightingSystem->DirLight.Direction;

	if (LightingSystem->Shadow.bIsShadowEnable)
	{
		UpdateDepthMap(RunningTime);
	}

	DrawLevel(RunningTime);

	if (MainView->bIsShowDepthMap)
	{
		DepthMapFrameBufferDebugDrawable->Draw(LightingSystem->Shadow.ProjectionNear,
			LightingSystem->Shadow.ProjectionFar);
	}

	if (MainView->bIsFrameBufferEnable)
	{
		FrameBuffer->PostProcessing = MainView->PostProcessing;
		FrameBuffer->Render([&]()
		{
			DrawLevel(RunningTime);
		});
	}
}

void FApp::DrawLevel(double RunningTime)
{
	std::vector<FStaticMesh*> StaticMeshs = { Mesh, FloorMesh, DirectionalLightMesh };
	std::vector<FGLStaticMeshDrawable*> StaticMeshDrawables = { Drawable, FloorMeshDrawable, DirectionalLightMeshDrawable };

	for (auto StaticMesh : StaticMeshs)
	{
		StaticMesh->View = Camera->GetViewMat();
		StaticMesh->Projection = Camera->GetprojectionMat();
	}

	for (auto StaticMeshDrawable : StaticMeshDrawables)
	{
		StaticMeshDrawable->Draw();
	}

	Skybox->View = glm::mat4(glm::mat3(Camera->GetViewMat()));
	Skybox->Projection = Camera->GetprojectionMat();
	SkyboxDrawable->Draw();

	SkeletonMesh->RunningTime = RunningTime;
	SkeletonMesh->View = Camera->GetViewMat();
	SkeletonMesh->Projection = Camera->GetprojectionMat();
	SkeletonMesh->UpdateBoneTransform();
	SkeletonMeshDrawable->Draw();
}

void FApp::UpdateDepthMap(double RunningTime)
{
	DepthMapFrameBuffer->Render([&]()
	{
		std::vector<FGLStaticMeshDrawable*> StaticMeshDrawables = { Drawable, FloorMeshDrawable };
		for (auto StaticMeshDrawable : StaticMeshDrawables)
		{
			FGLShader* Shader = DepthMapFrameBuffer->GetShareShader();
			StaticMeshDrawable->DrawWithDepthMapShader(*Shader);
		}
	});
}

void FApp::ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods)
{
	if (Action == GLFW_PRESS)
	{
		switch (Key)
		{
		case GLFW_KEY_ESCAPE:
			GLRenderer->SetWindowShouldClose();
			break;

		case GLFW_KEY_F1:
			Camera->bIsMovable = !Camera->bIsMovable;
			bMouseEnable = !bMouseEnable;

			if (bMouseEnable)
			{
				InputCenter->SetCursorMode(ECursorMode::Disable);
			}
			else
			{
				InputCenter->SetCursorMode(ECursorMode::Normal);
			}
			break;

		case GLFW_KEY_F2:
			MainView->bIsHidden = !MainView->bIsHidden;
			break;

		case GLFW_KEY_F:
			LightingSystem->SpotLight.bIsSpotLightEnable = !LightingSystem->SpotLight.bIsSpotLightEnable;
			break;

		case GLFW_KEY_M:
			const void* Data = GLRenderer->Capture();
			static int Index = 0;
			int FrameWidth = GLRenderer->GetScreenWidth();
			int FrameHeight = GLRenderer->GetScreenHeight();
			stbi_flip_vertically_on_write(1);
			std::ostringstream Oss;
			Oss << FileManager->GetResouceDir() << "/" << Index << ".jpg";
			const char* Filename = nullptr;
			std::string Str = Oss.rdbuf()->str();
			Filename = Str.c_str();
			int Result = stbi_write_jpg(Filename, FrameWidth, FrameHeight, 3, Data, FrameWidth * 3);
			spdlog::info("store image is: {}, store path: {}", Result ? "true" : "false", Str);
			Index = Index + 1;
			break;
		}
	}
}

void FApp::MouseCallback(GLFWwindow * Window, double Xpos, double Ypos)
{
}

void FApp::ContiguousInput(GLFWwindow * Window, int Key)
{
}

void FApp::ScrollCallback(GLFWwindow * Window, double Xoffset, double Yoffset)
{
}