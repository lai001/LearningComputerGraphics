#include "Image.h"
#include <unordered_map>
#include "ThirdParty/stb_image.h"
#include "ThirdParty/spdlog.h"

FImage::FImage()
{
}

FImage::FImage(const unsigned char & Data, const int Width, const int Height, const EImageFormatType ImageFormatType)
	:Width(Width), Height(Height), ImageFormatType(ImageFormatType)
{
	assert(ImageFormatType != EImageFormatType::Unknow);

	std::unordered_map<EImageFormatType, int> ChannelsDic;
	ChannelsDic[EImageFormatType::Gray] = 1;
	ChannelsDic[EImageFormatType::Rgb] = 3;
	ChannelsDic[EImageFormatType::Rgba] = 4;

	ColorChannels = ChannelsDic[ImageFormatType];
	size_t Size = ColorChannels * Width * Height;
	this->Data = new unsigned char[Size];
	memcpy(this->Data, &Data, Size);
}

FImage::~FImage()
{
	delete Data;
}

FImage * FImage::NewImageFromFilepath(const std::string & Filepath)
{
	stbi_set_flip_vertically_on_load(1);
	int Width;
	int Height;
	int ColorChannels;
	EImageFormatType ImageFormatType;
	const unsigned char* Data = stbi_load(Filepath.c_str(), &Width, &Height, &ColorChannels, 0);

	if (!Data)
	{
		return nullptr;
	}

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
		return nullptr;
	}
	spdlog::debug("load image successfully, width£º{}, height: {}, {}", Width, Height, Filepath);
	FImage* Image = new FImage(*Data, Width, Height, ImageFormatType);
	stbi_image_free((void*)Data);
	return Image;
}

unsigned char * const FImage::GetData() const
{
	return Data;
}

EImageFormatType FImage::GetImageFormatType() const
{
	return ImageFormatType;
}

int FImage::GetColorChannels() const
{
	return ColorChannels;
}

int FImage::GetWidth() const
{
	return Width;
}

int FImage::GetHeight() const
{
	return Height;
}
