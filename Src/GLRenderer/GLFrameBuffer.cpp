#include "GLFrameBuffer.h"
#include "ThirdParty/spdlog.h"

FGLFrameBuffer::FGLFrameBuffer(const int Width, const int Height, const glm::vec2 TopLeft, const glm::vec2 Size)
	:Width(Width), Height(Height)
{
	ColorTexture = FGLTexture::NewColorAttachmentTexture2D(Width, Height);
	glGenFramebuffers(1, &FramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
	AddColorAttachment();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Vertices = FGLFrameBuffer::GetVerticesFromRect(TopLeft, Size);
	VertexObject = FGLVertexObject::New(Vertices, []()
	{
		FGLVertexBufferLayout Layout;
		Layout.Float(2).Float(2);
		return Layout;
	});

	const std::string VertexShaderPath = FGLShader::GetShadersFolder().append("/GLFramebuffers.vert");
	const std::string FragmentShaderPath = FGLShader::GetShadersFolder().append("/GLFramebuffers.frag");
	this->Shader = FGLShader::New(VertexShaderPath, FragmentShaderPath);
}

FGLFrameBuffer::~FGLFrameBuffer()
{
	delete ColorTexture;
	delete Shader;
	delete VertexObject;
	glDeleteFramebuffers(1, &FramebufferID);
}

void FGLFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FGLFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
}

void FGLFrameBuffer::AddColorAttachment()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture->GetRendererID(), 0);
	glGenRenderbuffers(1, &RenderbuffersID);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderbuffersID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderbuffersID);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
}

const FGLTexture * FGLFrameBuffer::GetColorTexture() const
{
	return ColorTexture;
}

const FGLShader * FGLFrameBuffer::GetShader() const
{
	return Shader;
}

void FGLFrameBuffer::GLClearColor(const glm::vec4 Color)
{
	glClearColor(Color.r, Color.g, Color.b, Color.a);
}

void FGLFrameBuffer::ClearBuffer(const unsigned char ClearBufferFlags)
{
	if (ClearBufferFlags & EClearBufferFlags::Color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	if (ClearBufferFlags & EClearBufferFlags::Stencil)
	{
		glClear(GL_STENCIL_BUFFER_BIT);
	}
	if (ClearBufferFlags & EClearBufferFlags::Depth)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void FGLFrameBuffer::SetViewport(const int X, const int Y, const int Width, const int Height)
{
	glViewport(X, Y, Width, Height);
}

void FGLFrameBuffer::SetViewport(const glm::ivec4 & Viewport)
{
	FGLFrameBuffer::SetViewport(Viewport.x, Viewport.y, Viewport.z, Viewport.w);
}

void FGLFrameBuffer::Draw()
{
	//glViewport(0,0,100,100);
	VertexObject->Bind();
	Shader->Bind();
	const int Slot = 0;
	ColorTexture->Bind(Slot);
	Shader->SetUniform1i("screenTexture", Slot);
	Shader->SetUniform1i("postProcessing", PostProcessing);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FGLFrameBuffer::Render(std::function<void()> Closure)
{
	Bind();
	FGLFrameBuffer::GLClearColor(ClearColor);
	FGLFrameBuffer::ClearBuffer(ClearBufferFlags);
	//glEnable(GL_DEPTH_TEST);
	Closure();
	Unbind();
	Draw();
}

std::array<float, 24> FGLFrameBuffer::GetVerticesFromRect(const glm::vec2 TopLeft, const glm::vec2 Size)
{
	const glm::vec2 TopLeftUV = glm::vec2(0.0, 1.0);
	const glm::vec2 BottomLeftUV = glm::vec2(0.0, 0.0);
	const glm::vec2 TopRightUV = glm::vec2(1.0, 1.0);
	const glm::vec2 BottomRightUV = glm::vec2(1.0, 0.0);

	const glm::vec2 TopLeftPoint = glm::vec2(TopLeft.x, TopLeft.y);
	const glm::vec2 BottomLeftPoint = glm::vec2(TopLeft.x, TopLeft.y - Size.y);
	const glm::vec2 TopRightPoint = glm::vec2(TopLeft.x + Size.x, TopLeft.y);
	const glm::vec2 BottomRightPoint = glm::vec2(TopLeft.x + Size.x, TopLeft.y - Size.y);

	const std::array<glm::vec2, 12> _Vertices = {
		TopLeftPoint, TopLeftUV,
		BottomLeftPoint, BottomLeftUV,
		BottomRightPoint, BottomRightUV,

		TopLeftPoint, TopLeftUV,
		TopRightPoint, TopRightUV,
		BottomRightPoint, BottomRightUV
	};
	std::array<float, 24> Vertices;
	int i = 0;
	for (const glm::vec2 item : _Vertices)
	{
		Vertices[2 * i] = item.x;
		Vertices[2 * i + 1] = item.y;
		i++;
	}
	return Vertices;
}
