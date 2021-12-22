#pragma once
#include "GLRenderer/GLRenderer.h"
#include "GUI/MainView.h"
#include "LightingSystem/LightingSystem.h"
#include "Camera.h"
#include "StaticMesh.h"
#include "SkyBox.h"
#include "FileManager.h"
#include "SkeletonMesh.h"

class FApp : public IGLRunLoop, public IGLInput
{
public:
	FApp(int arg, char** args);
	~FApp();

private:
	FFileManager* FileManager;

	FGLRenderer* GLRenderer;

	FGLInputCenter* InputCenter;

	FLightingSystem* LightingSystem;

	FGUISystem* GUISystem;

	FMainView* MainView;

	FCamera* Camera;

	FSkyBox* Skybox;

	FGLSkyboxDrawable* SkyboxDrawable;

	FSkeletonMesh* SkeletonMesh;

	FGLSkeletonMeshDrawable* SkeletonMeshDrawable;

	bool bMouseEnable = true;

	FStaticMesh* Mesh;

	FGLStaticMeshDrawable* Drawable;

	FStaticMesh* FloorMesh;

	FGLStaticMeshDrawable* FloorMeshDrawable;

	FGLFrameBuffer* FrameBuffer;

	FGLDepthMapFrameBuffer* DepthMapFrameBuffer;
	FGLDepthMapFrameBufferDebugDrawable* DepthMapFrameBufferDebugDrawable;

	FStaticMesh* DirectionalLightMesh;
	FGLStaticMeshDrawable* DirectionalLightMeshDrawable;

	void DrawLevel(double RunningTime);

	void UpdateDepthMap(double RunningTime);

	void UpdateProperty(const FMainView& MainView);

public:
	virtual void Tick(const GLFWwindow& Window, double RunningTime) override;

public:
	virtual void ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods) override;

	virtual void MouseCallback(GLFWwindow * Window, double Xpos, double Ypos) override;

	virtual void ContiguousInput(GLFWwindow * Window, int Key) override;

	virtual void ScrollCallback(GLFWwindow * Window, double Xoffset, double Yoffset) override;

};