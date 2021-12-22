#pragma once
#include <string>
#include <functional>
#include <array>
#include "ThirdParty/opengl.h"
#include "ThirdParty/glm.h"
#include "ThirdParty/noncopyable.hpp"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLVertexObject.h"

enum EClearBufferFlags
{
	None = 1 << 0,
	Depth = 1 << 1,
	Color = 1 << 2,
	Stencil = 1 << 3
};

class FGLFrameBuffer: public boost::noncopyable
{

private:
	unsigned int FramebufferID;

	unsigned int RenderbuffersID;

	int Width;

	int Height;

	FGLVertexObject* VertexObject;

	FGLShader* Shader;

	FGLTexture* ColorTexture;

	std::array<float, 24> Vertices;

public:
	FGLFrameBuffer(const int Width, const int Height, const glm::vec2 TopLeft, const glm::vec2 Size);

	~FGLFrameBuffer();

	int PostProcessing = 0;

	glm::vec4 ClearColor;

	unsigned char ClearBufferFlags;

	void Unbind();

	void Bind();

	void AddColorAttachment();

	const FGLTexture* GetColorTexture() const;

	const FGLShader* GetShader() const;

	void Render(std::function<void()> Closure);

	void Draw();

	static std::array<float, 24> GetVerticesFromRect(const glm::vec2 TopLeft, const glm::vec2 Size);

	static void GLClearColor(const glm::vec4 Color);

	static void ClearBuffer(const unsigned char ClearBufferFlags);

	static void SetViewport(const int X, const int Y, const int Width, const int Height);
	static void SetViewport(const glm::ivec4& Viewport);
};
