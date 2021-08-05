#pragma once
#include <functional>
#include <vector>

#include "ThirdParty/opengl.h"

#include "GLIndexBuffer.h"
#include "GLInput.h"
#include "GLShader.h"
#include "GLSkyboxDrawable.h"
#include "GLTexture.h"
#include "GLVertexBufferLayout.h"
#include "GLVertexArray.h"
#include "GLVertexBuffer.h"
#include "GLRunLoop.h"
#include "GLInputCenter.h"
#include "GLStaticMeshDrawable.h"
#include "GLStaticSubMeshDrawable.h"
#include "GLFrameBuffer.h"
#include "GLSkeletonMeshDrawable.h"
#include "GLSkeletonSubMeshDrawable.h"

struct GLInfo
{
	const unsigned char* Version = nullptr;
	const unsigned char* Vendor = nullptr;
	const unsigned char* Renderer = nullptr;
};


class FGLRenderer
{

private:
	int ScreenWidth = 800;
	int ScreenHeight = 600;
	GLFWwindow* Window;
	FGLInputCenter* InputCenter;
	std::vector<IGLRunLoop*> RunLoops;

public:
	FGLRenderer(int Width, int Height, const char * Title);
	~FGLRenderer();

public:
	int GetScreenWidth();
	int GetScreenHeight();
	GLFWwindow* GetWindow();
	FGLInputCenter* GetInputCenter();
	const void* Capture();

public:
	void MakeContextCurrent();
	void SwapInterval(int Interval);
	static void SetFramebufferSizeCallback(GLFWwindow* Window, GLFWframebuffersizefun cbfun);
	void StartMainLoop();
	void Terminate();
	void SetWindowShouldClose();
	void AddRunLoop(IGLRunLoop* RunLoop);
	void RemoveRunLoop(IGLRunLoop* RunLoop);
	const GLInfo GetGLInfo();

};

