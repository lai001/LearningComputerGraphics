#pragma once
#include <string>
#include "ThirdParty/noncopyable.hpp"
enum class EImageFormatType
{
	Unknow,
	Gray,
	Rgb,
	Rgba
};

class FImage: public boost::noncopyable
{
protected:
	FImage();
public:
	FImage(const unsigned char& Data, const int Width, const int Height, const EImageFormatType ImageFormatType);
	~FImage();

	static FImage* NewImageFromFilepath(const std::string& Filepath);

	unsigned char* const GetData() const;
	EImageFormatType GetImageFormatType() const;
	int GetColorChannels() const;
	int GetWidth() const;
	int GetHeight() const;

protected:
	unsigned char* Data = nullptr;
	int Width;
	int Height;
	int ColorChannels;
	EImageFormatType ImageFormatType;
};
