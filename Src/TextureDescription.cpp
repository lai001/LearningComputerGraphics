#include "TextureDescription.h"
#include <iostream>
#include "ThirdParty/spdlog.h"
#include "ThirdParty/stb_image.h"

FTextureDescription::FTextureDescription()
{
}

FTextureDescription::~FTextureDescription()
{
	delete Image;
}

FTextureDescription * FTextureDescription::New(const std::string & ImageFilePath, const aiTextureType TextureType)
{
	FImage* Image = FImage::NewImageFromFilepath(ImageFilePath);
	if (Image == nullptr)
	{
		return nullptr;
	}
	FTextureDescription* TextureDescription = new FTextureDescription();
	TextureDescription->Filepath = ImageFilePath;
	TextureDescription->TextureType = TextureType;
	TextureDescription->Image = Image;
	return TextureDescription;
}

aiTextureType FTextureDescription::GetTextureType() const
{
	return TextureType;
}

const unsigned char * FTextureDescription::GetImageBuffer() const
{
	return Image->GetData();
}

EImageFormatType FTextureDescription::GetImageFormatType() const
{
	return Image->GetImageFormatType();
}

int FTextureDescription::GetColorChannels() const
{
	return Image->GetColorChannels();
}

int FTextureDescription::GetWidth() const
{
	return Image->GetWidth();
}

int FTextureDescription::GetHeight() const
{
	return Image->GetHeight();
}
