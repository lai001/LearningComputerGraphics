#include "GLFrameBuffer.h"
#include "ThirdParty/spdlog.h"

FGLFrameBuffer::FGLFrameBuffer(int Width, int Height, std::string VertexShaderPath, std::string FragmentShaderPath)
	:Width(Width), Height(Height), VertexShaderPath(VertexShaderPath), FragmentShaderPath(FragmentShaderPath)
{
	glGenFramebuffers(1, &RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
	AddColorAttachment();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->Vao = new FGLVertexArray();
	this->Vbo = new FGLVertexBuffer(StandardUVMapVertices, sizeof(StandardUVMapVertices));
	FGLVertexBufferLayout Layout;
	Layout.Push<float>(2);
	Layout.Push<float>(2);
	Vao->AddBuffer(Vbo, Layout);
	this->Shader = new FGLShader(VertexShaderPath, FragmentShaderPath);
}

FGLFrameBuffer::~FGLFrameBuffer()
{
	delete Shader;
	delete Vao;
	delete Vbo;
	glDeleteFramebuffers(1, &RendererID);
}

void FGLFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FGLFrameBuffer::Bind(std::function<void()> RenderClosure)
{
	glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
	RenderClosure();
	Unbind();
}

void FGLFrameBuffer::AddColorAttachment()
{
	glGenTextures(1, &ColorTexture);
	glBindTexture(GL_TEXTURE_2D, ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture, 0);

	glGenRenderbuffers(1, &Rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
}

unsigned int FGLFrameBuffer::GetColorTexture()
{
	return ColorTexture;
}

void FGLFrameBuffer::GLClearColor(glm::vec4 Color)
{
	glClearColor(Color.r, Color.g, Color.b, Color.a);
}

void FGLFrameBuffer::ClearBuffer(unsigned char ClearBufferFlags)
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

FGLShader * FGLFrameBuffer::GetShader()
{
	return Shader;
}

void FGLFrameBuffer::Draw()
{
	Vao->Bind();
	Shader->Bind();
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, GetColorTexture());
	Shader->SetUniform1i("screenTexture", 0);
	Shader->SetUniform1i("postProcessing", PostProcessing);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FGLFrameBuffer::Render(std::function<void()> Closure)
{
	Bind([&]() {
		FGLFrameBuffer::GLClearColor(ClearColor);
		FGLFrameBuffer::ClearBuffer(ClearBufferFlags);
		//glEnable(GL_DEPTH_TEST);
		Closure();
	});
	Draw();
}
