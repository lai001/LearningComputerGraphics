#include "GLRenderer.h"
#include "GLInputCenter.h"

FGLRenderer::FGLRenderer(int Width, int Height, const char * Title)
	:ScreenWidth(Width), ScreenHeight(Height)
{
	assert(glfwInit());
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window = glfwCreateWindow(Width, Height, Title, NULL, NULL);
	assert(Window);
	InputCenter = new FGLInputCenter(Window);

	SwapInterval(1);
}

FGLRenderer::~FGLRenderer()
{
	delete InputCenter;
}

int FGLRenderer::GetScreenWidth()
{
	return ScreenWidth;
}

int FGLRenderer::GetScreenHeight()
{
	return ScreenHeight;
}

GLFWwindow* FGLRenderer::GetWindow()
{
	return Window;
}

FGLInputCenter * FGLRenderer::GetInputCenter()
{
	return InputCenter;
}

const void * FGLRenderer::Capture()
{
	int ScreenWidth = GetScreenWidth();
	int ScreenHeight = GetScreenHeight();
	static int Index = 0;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, ScreenWidth, ScreenHeight);
	int FrameWidth = ScreenWidth;
	int FrameHeight = ScreenHeight;
	unsigned int ReadBuffer;
	glGenBuffers(1, &ReadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ReadBuffer);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int)sizeof(char) * FrameWidth * FrameHeight * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, ReadBuffer);
	glReadPixels(0, 0, FrameWidth, FrameHeight, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	void *Data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	return Data;
}

void FGLRenderer::MakeContextCurrent()
{
	glfwMakeContextCurrent(Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void FGLRenderer::SwapInterval(int Interval=1)
{
	glfwSwapInterval(Interval);
}

void FGLRenderer::SetFramebufferSizeCallback(GLFWwindow* Window, GLFWframebuffersizefun cbfun)
{
	glfwSetFramebufferSizeCallback(Window, cbfun);
}

void FGLRenderer::StartMainLoop()
{
	while (!glfwWindowShouldClose(Window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double Time = glfwGetTime();
		for (IGLRunLoop* RunLoop : RunLoops)
		{
			RunLoop->Tick(*Window, Time);
		}

		InputCenter->ContiguousInput();

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	Terminate();
}

void FGLRenderer::Terminate()
{
	glfwTerminate();
}

void FGLRenderer::SetWindowShouldClose()
{
	glfwSetWindowShouldClose(Window, GL_TRUE);
}

void FGLRenderer::AddRunLoop(IGLRunLoop * RunLoop)
{
	assert(RunLoop);
	RunLoops.push_back(RunLoop);
}

void FGLRenderer::RemoveRunLoop(IGLRunLoop * RunLoop)
{
	assert(RunLoop);
	for (std::vector<IGLRunLoop*>::iterator Iter = RunLoops.begin(); Iter != RunLoops.end();)
	{
		if (*Iter == RunLoop)
		{
			Iter = RunLoops.erase(Iter);
		}
		else
		{
			Iter = Iter + 1;
		}
	}
}

const GLInfo FGLRenderer::GetGLInfo()
{
	GLInfo Info;
	Info.Version = glGetString(GL_VERSION);
	Info.Vendor = glGetString(GL_VENDOR);
	Info.Renderer = glGetString(GL_RENDERER);
	return Info;
}
