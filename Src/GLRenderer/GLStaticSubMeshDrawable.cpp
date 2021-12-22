#include "GLStaticSubMeshDrawable.h"
#include "ThirdParty/spdlog.h"
#include "Util.h"

FGLStaticSubMeshDrawable::FGLStaticSubMeshDrawable(const FStaticSubMesh* SubMesh)
	:SubMesh(SubMesh)
{
	assert(SubMesh);
	VertexObject = FGLVertexObject::New(SubMesh->Vertices, SubMesh->Indices, []()
	{
		FGLVertexBufferLayout Layout;
		Layout.Float(3).Float(3).Float(2);
		return Layout;
	});
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
			assert(false);
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
	delete VertexObject;
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
	VertexObject->Bind();
	glDrawElements(GL_TRIANGLES, SubMesh->Indices.size(), GL_UNSIGNED_INT, 0);
}
