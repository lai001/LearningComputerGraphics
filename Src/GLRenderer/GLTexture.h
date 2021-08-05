#pragma once
#include <string>
#include "ThirdParty/assimp.h"
#include "ThirdParty/opengl.h"

class FGLTexture
{

private:
	unsigned int RendererID;
	int Width;
	int Height;
	int ColorChannels;
	aiTextureType TextureType;
	GLenum Format;

public:
	FGLTexture(GLenum Format, aiTextureType TextureType, int Width, int Height, int ColorChannels, const void* LocalBuffer);
	~FGLTexture();

	aiTextureType GetTextureType();

	void Bind(unsigned int Slot) const;
	void UnBind();

	int GetWidth() const;
	int GetHeight() const;
};

