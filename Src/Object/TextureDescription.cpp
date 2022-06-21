#include "TextureDescription.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <assimp/material.h>
#include "ImageIO/ImageIO.hpp"

FTextureDescription::~FTextureDescription()
{
	delete PixelBuffer;
}

FTextureDescription * FTextureDescription::New(
	const std::string & ImageFilePath, 
	const aiTextureType TextureType)
{
	FTextureDescription* TextureDescription = new FTextureDescription();
	TextureDescription->PixelBuffer = ImageIO::ReadImageFromFilePath(ImageFilePath);
	TextureDescription->Filepath = ImageFilePath;
	TextureDescription->TextureType = TextureType;
	return TextureDescription;
}

aiTextureType FTextureDescription::GetTextureType() const
{
	return TextureType;
}

std::string FTextureDescription::GetFilepath() const
{
	return Filepath;
}

const ks::PixelBuffer * FTextureDescription::GetPixelBuffer() const noexcept
{
	return PixelBuffer;
}
