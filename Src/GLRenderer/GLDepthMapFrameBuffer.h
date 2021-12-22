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

class FGLDepthMapFrameBuffer: public boost::noncopyable
{
public:
	FGLDepthMapFrameBuffer(const int Width, const int Height);

	~FGLDepthMapFrameBuffer();

	void Unbind();

	void Bind();

	void Render(std::function<void()> Closure);

	int GetWidth() const;

	int GetHeight() const;

	const FGLTexture* GetTexture() const;

	FGLShader* GetShareShader() const;

private:
	int Width;

	int Height;

	FGLTexture* DepthMapTexture;

	unsigned int FramebufferID;

	FGLShader* ShareShader;
};