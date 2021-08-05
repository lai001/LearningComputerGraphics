#include "GLStaticSubMeshDrawable.h"

FGLStaticSubMeshDrawable::FGLStaticSubMeshDrawable(FStaticSubMesh* SubMesh)
	:SubMesh(SubMesh)
{
	Va = new FGLVertexArray();
	Vb = new FGLVertexBuffer(SubMesh->Vertices.data(), SubMesh->Vertices.size() * sizeof(FBaseVertex));
	Ib = new FGLIndexBuffer(SubMesh->Indices.data(), SubMesh->Indices.size());
	Layout = new FGLVertexBufferLayout();
	Layout->Push<float>(3);
	Layout->Push<float>(3);
	Layout->Push<float>(2);
	Va->AddBuffer(Vb, *Layout);

	for (int i = 0; i < SubMesh->Textures.size(); i++)
	{
		FTextureDescription* TextureDes = SubMesh->Textures[i];
		GLenum ImageFormat;
		switch (TextureDes->GetImageFormatType())
		{
		case EImageFormatType::Gray:
			ImageFormat = GL_RED;
			break;
		case EImageFormatType::Rgb:
			ImageFormat = GL_RGB;
			break;
		case EImageFormatType::Rgba:
			ImageFormat = GL_RGBA;
			break;
		case EImageFormatType::Unknow:
			__debugbreak();
			break;
		}
		FGLTexture* Texture = new FGLTexture(ImageFormat,
											TextureDes->GetTextureType(),
											TextureDes->GetWidth(),
											TextureDes->GetHeight(),
											TextureDes->GetColorChannels(),
											TextureDes->GetImageBuffer());
		Textures.push_back(Texture);
	}
}

FGLStaticSubMeshDrawable::~FGLStaticSubMeshDrawable()
{
	delete Va;
	delete Vb;
	delete Layout;
	delete Ib;
	for (auto item : Textures)
	{
		delete item;
	}
}

void FGLStaticSubMeshDrawable::Draw(FGLShader* Shader)
{
	for (int i = 0; i < Textures.size(); i++)
	{
		FGLTexture* texture = Textures[i];
		texture->Bind(i);
		Shader->SetTexture(texture->GetTextureType(), i);
	}

	Va->Bind();
	glDrawElements(GL_TRIANGLES, SubMesh->Indices.size(), GL_UNSIGNED_INT, 0);
}
