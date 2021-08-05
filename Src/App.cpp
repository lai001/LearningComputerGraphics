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

	GLRenderer = new FGLRenderer(1280, 720, "Nut");
	InputCenter = GLRenderer->GetInputCenter();
	LightingSystem = new FLightingSystem();
	GUISystem = new FGUISystem(GLRenderer->GetWindow());
	NutGUI = new FNutGUI();
	GUISystem->Interface = NutGUI;

	Camera = new FCamera(GLRenderer->GetScreenWidth(), GLRenderer->GetScreenHeight());

	InputCenter->AddInput(this);
	InputCenter->AddInput(Camera);

	GLRenderer->AddRunLoop(this);
	GLRenderer->AddRunLoop(GUISystem);

	GLRenderer->MakeContextCurrent();

	GLInfo Info = GLRenderer->GetGLInfo();
	spdlog::debug("{}, {}, {}", Info.Version, Info.Vendor, Info.Renderer);

	{
		Mesh = new FStaticMesh(FileManager->GetResourcePath("untitled.dae"));
		Mesh->VertexShaderFilePath = FileManager->GetResourcePath("GLShaders/GLStaticMesh.vert");
		Mesh->FragmentShaderFilePath = FileManager->GetResourcePath("GLShaders/GLStaticMesh.frag");

		glm::mat4 Model;
		Model = glm::translate(Model, LightingSystem->PointLight.Position);
		Model = glm::translate(Model, glm::vec3(0, 1.0, 0));
		Model = glm::scale(Model, glm::vec3(0.2f));

		Mesh->Model = Model;
		Mesh->View = Camera->GetViewMat();
		Mesh->Projection = Camera->GetprojectionMat();
		Mesh->LightingSystem = LightingSystem;
		Drawable = new FGLStaticMeshDrawable(Mesh);
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

		Skybox = new FSkyBox(SkyBoxFilePaths,
			FileManager->GetResourcePath("GLShaders/GLSkybox.vert"),
			FileManager->GetResourcePath("GLShaders/GLSkybox.frag"));
		SkyboxDrawable = new FGLSkyboxDrawable(Skybox);
	}

	{
		FrameBuffer = new FGLFrameBuffer(1280, 720,
			FileManager->GetResourcePath("GLShaders/GLFramebuffers.vert"),
			FileManager->GetResourcePath("GLShaders/GLFramebuffers.frag"));
		FrameBuffer->ClearColor = glm::vec4(0.3f, 0.4f, 0.5f, 1.0f);
		FrameBuffer->ClearBufferFlags = EClearBufferFlags::Color | EClearBufferFlags::Depth;
	}

	{
		SkeletonMesh = new FSkeletonMesh(FileManager->GetResourcePath("skeletal_animation/model.dae"));
		SkeletonMesh->VertexShaderFilePath = FileManager->GetResourcePath("GLShaders/GLSkeletonMesh.vert");
		SkeletonMesh->FragmentShaderFilePath = FileManager->GetResourcePath("GLShaders/GLSkeletonMesh.frag");

		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(0.0f, -0.9f, 0.0f));
		Model = glm::scale(Model, glm::vec3(0.2f, 0.2f, 0.2f));
		Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));

		SkeletonMesh->Model = Model;
		SkeletonMesh->View = Camera->GetViewMat();
		SkeletonMesh->Projection = Camera->GetprojectionMat();
		SkeletonMesh->Speed = 1.0;
		SkeletonMesh->LightingSystem = LightingSystem;

		SkeletonMeshDrawable = new FGLSkeletonMeshDrawable(SkeletonMesh);
	}

	GLRenderer->StartMainLoop();
}

FApp::~FApp()
{
	delete GLRenderer;
	delete LightingSystem;
	delete GUISystem;
	delete NutGUI;
	delete Camera;
	delete Mesh;
	delete Drawable;
	delete FrameBuffer;
	delete FileManager;
}

void FApp::Tick(GLFWwindow * Window, double RunningTime)
{
	LightingSystem->SpotLight.Position = Camera->GetPosition();
	LightingSystem->SpotLight.Direction = Camera->GetCameraFront();
	LightingSystem->bIsSpotLightEnable = bIsSpotLightEnable;

	NutGUI->CurrentFOV = Camera->GetFov();
	NutGUI->CurrentCameraSpeed = Camera->GetCameraSpeed();

	DrawLevel(RunningTime);

	if (NutGUI->bIsFrameBufferEnable)
	{
		FrameBuffer->PostProcessing = NutGUI->PostProcessing;
		FrameBuffer->Render([&]() {
			DrawLevel(RunningTime);
		});
	}
}

void FApp::DrawLevel(double RunningTime) {
	Mesh->View = Camera->GetViewMat();
	Mesh->Projection = Camera->GetprojectionMat();
	Drawable->Draw();

	if (NutGUI->bIsShowSkybox)
	{
		Skybox->View = glm::mat4(glm::mat3(Camera->GetViewMat()));
		Skybox->Projection = Camera->GetprojectionMat();
		SkyboxDrawable->Draw();
	}

	if (SkeletonMesh)
	{
		SkeletonMesh->bIsEnableBindingPost = NutGUI->BindingPostEnable;
		SkeletonMesh->RunningTime = RunningTime;
		SkeletonMesh->Speed = NutGUI->ModelAnimationSpeed;
		SkeletonMesh->View = Camera->GetViewMat();
		SkeletonMesh->Projection = Camera->GetprojectionMat();
		SkeletonMesh->UpdateBoneTransform();
		SkeletonMeshDrawable->Draw();
	}
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

		case GLFW_KEY_F:
			bIsSpotLightEnable = !bIsSpotLightEnable;
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