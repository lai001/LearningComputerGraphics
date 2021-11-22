#include "GLSkyboxDrawable.h"
#include "ThirdParty/stb_image.h"
#include "ThirdParty/spdlog.h"

FGLSkyboxDrawable::FGLSkyboxDrawable(FSkyBox* Skybox)
	:Skybox(Skybox)
{
	this->Vao = new FGLVertexArray();
	this->Vbo = new FGLVertexBuffer(SkyboxVertices, sizeof(SkyboxVertices));
	FGLVertexBufferLayout Layout;
	Layout.Push<float>(3);
	Vao->AddBuffer(Vbo, Layout);
	const std::string VertexShaderPath = FGLShader::GetShadersFolder().append("/GLSkybox.vert");
	const std::string FragmentShaderPath = FGLShader::GetShadersFolder().append("/GLSkybox.frag");
	this->Shader = new FGLShader(VertexShaderPath, FragmentShaderPath);
	LoadSkyBox();
}

FGLSkyboxDrawable::~FGLSkyboxDrawable()
{
	delete Shader;
	delete Vao;
	delete Vbo;
}

void FGLSkyboxDrawable::LoadSkyBox()
{
	unsigned int RendererID;
	glGenTextures(1, &RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);

	int Width;
	int Height;
	int ColorChannels;

	int LoadCount = 0;
	
	for (int i = 0; i < Skybox->FilePaths.size(); i++)
	{
		const char* FilePath = Skybox->FilePaths[i].c_str();
		unsigned char* ImageData = stbi_load(FilePath, &Width, &Height, &ColorChannels, 0);

		if (ImageData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
			stbi_image_free(ImageData);
			LoadCount = LoadCount + 1;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (LoadCount == Skybox->FilePaths.size())
	{
		spdlog::info("load skybox successfully");
	}
	else
	{
		spdlog::error("load skybox failed");
	}

	this->RendererID = RendererID;
}


void FGLSkyboxDrawable::Draw()
{
	glDepthFunc(GL_LEQUAL);
	Shader->Bind();

	glm::mat4 Projection = Skybox->Projection;
	glm::mat4 View = Skybox->View;
	Shader->SetMatrix(glm::mat4(1.0f), View, Projection);

	Vao->Bind();
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);
	Shader->SetUniform1i("skybox", 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}
