#include "TextureDescription.h"
#include <iostream>
#include "ThirdParty/spdlog.h"
#include "ThirdParty/stb_image.h"

FTextureDescription::FTextureDescription(const std::string & ImageFilePath, aiTextureType TextureType)
	:Filepath(ImageFilePath), LocalBuffer(nullptr), Width(0), Height(0), ColorChannels(0), TextureType(TextureType)
{
	stbi_set_flip_vertically_on_load(1);
	LocalBuffer = stbi_load(ImageFilePath.c_str(), &Width, &Height, &ColorChannels, 4);

	if (LocalBuffer)
	{
		spdlog::debug("load image successfully, width£º{}, height: {}, {}, textureType: {}", Width, Height, ImageFilePath, TextureType);
	}
	else
	{
		spdlog::error("load image failed!");
		__debugbreak();
	}

	if (LocalBuffer)
	{
		if (ColorChannels == 1)
		{
			ImageFormatType = EImageFormatType::Gray;
		}
		else if (ColorChannels == 3)
		{
			ImageFormatType = EImageFormatType::Rgb;
		}
		else if (ColorChannels == 4)
		{
			ImageFormatType = EImageFormatType::Rgba;
		}
		else
		{
			__debugbreak();
		}
	}
}

FTextureDescription::~FTextureDescription()
{
	if (LocalBuffer)
	{
		stbi_image_free(LocalBuffer);
		LocalBuffer = nullptr;
	}
}

aiTextureType FTextureDescription::GetTextureType()
{
	return TextureType;
}

const unsigned char * FTextureDescription::GetImageBuffer()
{
	if (LocalBuffer)
	{
		return LocalBuffer;
	}
	return nullptr;
}

EImageFormatType FTextureDescription::GetImageFormatType()
{
	return ImageFormatType;
}

int FTextureDescription::GetColorChannels()
{
	return ColorChannels;
}

int FTextureDescription::GetWidth() const
{
	return Width;
}

int FTextureDescription::GetHeight() const
{
	return Height;
}
