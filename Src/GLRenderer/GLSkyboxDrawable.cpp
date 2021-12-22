#include "GLSkyboxDrawable.h"
#include <vector>
#include "ThirdParty/stb_image.h"
#include "ThirdParty/spdlog.h"

FGLSkyboxDrawable::FGLSkyboxDrawable(FSkyBox* Skybox)
	:Skybox(Skybox)
{
	this->Vao = new FGLVertexArray();
	this->Vbo = new FGLVertexBuffer(SkyboxVertices, sizeof(SkyboxVertices));
	FGLVertexBufferLayout Layout;
	Layout.Float(3);
	Vao->AddBuffer(*Vbo, Layout);
	const std::string VertexShaderPath = FGLShader::GetShadersFolder().append("/GLSkybox.vert");
	const std::string FragmentShaderPath = FGLShader::GetShadersFolder().append("/GLSkybox.frag");
	this->Shader = FGLShader::New(VertexShaderPath, FragmentShaderPath);
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
	assert(Skybox->FilePaths.size() == 6);

	int Width = 0;
	int Height = 0;
	int ColorChannels = 0;

	std::vector<void*> ImageDatas;
	for (int i = 0; i < Skybox->FilePaths.size(); i++)
	{
		const char* FilePath = Skybox->FilePaths[i].c_str();
		unsigned char* ImageData = stbi_load(FilePath, &Width, &Height, &ColorChannels, 0);
		assert(ImageData);
		ImageDatas.push_back(ImageData);
	}
	assert(ColorChannels == 3 || ColorChannels == 4);
	std::vector<const void*> _ImageDatas;
	for (void* ImageData : ImageDatas)
	{
		_ImageDatas.push_back(ImageData);
	}
	CubeMapTexture = FGLTexture::NewTextureCubeMap(Width, Height, GLPixelFormatType::RGBA, _ImageDatas);
	for (int i = 0; i < ImageDatas.size(); i++)
	{
		stbi_image_free(ImageDatas[i]);
	}
}

void FGLSkyboxDrawable::Draw()
{
	if (Skybox->bIsVisible == false)
	{
		return;
	}
	glDepthFunc(GL_LEQUAL);
	Shader->Bind();
	glm::mat4 Projection = Skybox->Projection;
	glm::mat4 View = Skybox->View;
	Shader->SetMatrix(glm::mat4(1.0f), View, Projection);
	Vao->Bind();
	CubeMapTexture->Bind(0);
	Shader->SetUniform1i("skybox", 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}
