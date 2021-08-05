#pragma once
#include <string>
#include "ThirdParty/assimp.h"

enum class EImageFormatType
{
	Unknow,
	Gray,
	Rgb,
	Rgba
};

class FTextureDescription
{
public:
	FTextureDescription(const std::string& ImageFilePath, aiTextureType TextureType);
	~FTextureDescription();

	aiTextureType GetTextureType();
	const unsigned char* GetImageBuffer();
	EImageFormatType GetImageFormatType();
	int GetColorChannels();
	int GetWidth() const;
	int GetHeight() const;

private:
	std::string Filepath;
	unsigned char* LocalBuffer;
	int Width;
	int Height;
	int ColorChannels;
	aiTextureType TextureType;
	EImageFormatType ImageFormatType;
};