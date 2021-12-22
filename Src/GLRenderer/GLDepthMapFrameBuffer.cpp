#include "GLDepthMapFrameBuffer.h"
#include "GLFrameBuffer.h"

FGLDepthMapFrameBuffer::FGLDepthMapFrameBuffer(const int Width, const int Height)
	:Width(Width), Height(Height)
{
	DepthMapTexture = FGLTexture::NewDepthMapTexture2D(Width, Height);
	glGenFramebuffers(1, &FramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMapTexture->GetRendererID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	ShareShader = FGLShader::New(FGLShader::GetShadersFolder().append("/GLDepthMap.vert"),
		FGLShader::GetShadersFolder().append("/GLDepthMap.frag"));
}

FGLDepthMapFrameBuffer::~FGLDepthMapFrameBuffer()
{
	delete DepthMapTexture;
	delete ShareShader;
	glDeleteFramebuffers(1, &FramebufferID);
}

void FGLDepthMapFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FGLDepthMapFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
}

void FGLDepthMapFrameBuffer::Render(std::function<void()> Closure)
{
	//FGLFrameBuffer::SetViewport(0, 0, Width, Height);
	glEnable(GL_DEPTH_TEST);
	Bind();
	FGLFrameBuffer::ClearBuffer(EClearBufferFlags::Depth | EClearBufferFlags::Color);
	Closure();
	Unbind();
}

int FGLDepthMapFrameBuffer::GetWidth() const
{
	return Width;
}

int FGLDepthMapFrameBuffer::GetHeight() const
{
	return Height;
}

const FGLTexture * FGLDepthMapFrameBuffer::GetTexture() const
{
	return DepthMapTexture;
}

FGLShader * FGLDepthMapFrameBuffer::GetShareShader() const
{
	return ShareShader;
}
