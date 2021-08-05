#include "GLTexture.h"
#include <iostream>
#include "ThirdParty/spdlog.h"
#include "ThirdParty/stb_image.h"

FGLTexture::FGLTexture(GLenum Format, aiTextureType TextureType, int Width, int Height, int ColorChannels, const void* LocalBuffer)
	:Width(Width), Height(Height), Format(Format), ColorChannels(ColorChannels), TextureType(TextureType), RendererID(0)
{
	if (LocalBuffer)
	{
		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, LocalBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	} 
	else
	{
		spdlog::error("load image failed!");
		__debugbreak();
	}
}

FGLTexture::~FGLTexture()
{
	glDeleteTextures(1, &RendererID);
}

aiTextureType FGLTexture::GetTextureType()
{
	return TextureType;
}

void FGLTexture::Bind(unsigned int Slot) const
{
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(GL_TEXTURE_2D, RendererID);
}

void FGLTexture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int FGLTexture::GetWidth() const
{
	return Width;
}

int FGLTexture::GetHeight() const
{
	return Height;
}
