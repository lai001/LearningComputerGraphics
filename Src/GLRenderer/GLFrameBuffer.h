#pragma once
#include <string>
#include <functional>
#include "ThirdParty/opengl.h"
#include "ThirdParty/glm.h"
#include "ThirdParty/bitmask_operators.hpp"
#include "GLVertexArray.h"
#include "GLShader.h"

enum EClearBufferFlags
{
	None = 1 << 0,
	Depth = 1 << 1,
	Color = 1 << 2,
	Stencil = 1 << 3
};

class FGLFrameBuffer
{

private:
	unsigned int RendererID;

	unsigned int ColorTexture;

	unsigned int Rbo;

	int Width;

	int Height;

	//std::string VertexShaderPath;

	//std::string FragmentShaderPath;

	FGLVertexArray* Vao;

	FGLVertexBuffer* Vbo;

	FGLShader* Shader;

	float StandardUVMapVertices[24] = {
	 0.5f,  1.0f,  0.0f, 1.0f,
	 0.5,   0.5f,  0.0f, 0.0f,
	 1.0f,  0.5f,  1.0f, 0.0f,

	 0.5f,  1.0f,  0.0f, 1.0f,
	 1.0f,  0.5f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	void Draw();

	static void GLClearColor(glm::vec4 Color);

	static void ClearBuffer(unsigned char ClearBufferFlags);

public:
	FGLFrameBuffer(int Width, int Height);
	~FGLFrameBuffer();

	int PostProcessing = 0;

	glm::vec4 ClearColor;

	unsigned char ClearBufferFlags;

	void Unbind();

	void Bind(std::function<void()> RenderClosure);

	void AddColorAttachment();

	unsigned int GetColorTexture();

	FGLShader* GetShader();

	void Render(std::function<void()> Closure);
};
