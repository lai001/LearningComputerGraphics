#include "GLDepthMapFrameBufferDebugDrawable.h"
#include "GLFrameBuffer.h"

FGLDepthMapFrameBufferDebugDrawable::FGLDepthMapFrameBufferDebugDrawable(const FGLDepthMapFrameBuffer& Framebuffer,
	const glm::vec2 TopLeft,
	const glm::vec2 Size)
	:Framebuffer(&Framebuffer)
{
	std::array<float, 24> Vertices = FGLFrameBuffer::GetVerticesFromRect(TopLeft, Size);
	VertexObject = FGLVertexObject::New(Vertices, []()
	{
		FGLVertexBufferLayout Layout;
		Layout.Float(2).Float(2);
		return Layout;
	});
	const std::string VertexShaderPath = FGLShader::GetShadersFolder().append("/GLDepthMapDebug.vert");
	const std::string FragmentShaderPath = FGLShader::GetShadersFolder().append("/GLDepthMapDebug.frag");
	DrawShader = FGLShader::New(VertexShaderPath, FragmentShaderPath);
}

FGLDepthMapFrameBufferDebugDrawable::~FGLDepthMapFrameBufferDebugDrawable()
{
	delete VertexObject;
	delete DrawShader;
}

void FGLDepthMapFrameBufferDebugDrawable::Draw(const float Near, const float Far)
{
	VertexObject->Bind();
	DrawShader->Bind();
	const int Slot = 0;
	const FGLTexture* DepthMapTexture = Framebuffer->GetTexture();
	DepthMapTexture->Bind(Slot);
	DrawShader->SetUniform1i("depthMap", Slot);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	DrawShader->Unbind();
	VertexObject->Unbind();
}