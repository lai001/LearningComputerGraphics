#pragma once
#include "ThirdParty/glm.h"
#include "ThirdParty/noncopyable.hpp"
#include "GLDepthMapFrameBuffer.h"

class FGLDepthMapFrameBufferDebugDrawable : public boost::noncopyable
{
public:
	FGLDepthMapFrameBufferDebugDrawable(const FGLDepthMapFrameBuffer & Framebuffer, 
		const glm::vec2 TopLeft,
		const glm::vec2 Size);
	~FGLDepthMapFrameBufferDebugDrawable();
	void Draw(const float Near, const float Far);

private:
	const FGLDepthMapFrameBuffer* Framebuffer;
	FGLShader* DrawShader;

	FGLVertexObject* VertexObject;
};