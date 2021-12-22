#pragma once
#include <string>
#include "ThirdParty/assimp.h"
#include "ThirdParty/noncopyable.hpp"
#include "Image.h"

class FTextureDescription: public boost::noncopyable
{
protected:
	FTextureDescription();

public:
	~FTextureDescription();

	static FTextureDescription* New(const std::string& ImageFilePath, const aiTextureType TextureType);

	aiTextureType GetTextureType() const;
	const unsigned char* GetImageBuffer() const;
	EImageFormatType GetImageFormatType() const;
	int GetColorChannels() const;
	int GetWidth() const;
	int GetHeight() const;

protected:
	std::string Filepath;
	FImage* Image = nullptr;
	aiTextureType TextureType;
};