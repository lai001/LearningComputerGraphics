#pragma once
#include "GLRenderer/GLRenderer.h"
#include "GUI/NutGUI.h"
#include "LightingSystem/LightingSystem.h"
#include "Camera.h"
#include "StaticMesh.h"
#include "SkyBox.h"
#include "FileManager.h"
#include "SkeletonMesh.h"

class FApp: public IGLRunLoop, public IGLInput
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

	FNutGUI* NutGUI;

	FCamera* Camera;

	FSkyBox* Skybox;

	FGLSkyboxDrawable* SkyboxDrawable;

	FSkeletonMesh* SkeletonMesh;

	FGLSkeletonMeshDrawable* SkeletonMeshDrawable;

	bool bIsSpotLightEnable = true;

	bool bMouseEnable = true;

	FStaticMesh* Mesh;

	FGLStaticMeshDrawable* Drawable;

	FGLFrameBuffer* FrameBuffer;

	void DrawLevel(double RunningTime);

public:
	virtual void Tick(GLFWwindow * Window, double RunningTime) override;

public:
	virtual void ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods) override;

	virtual void MouseCallback(GLFWwindow * Window, double Xpos, double Ypos) override;

	virtual void ContiguousInput(GLFWwindow * Window, int Key) override;

	virtual void ScrollCallback(GLFWwindow * Window, double Xoffset, double Yoffset) override;

};