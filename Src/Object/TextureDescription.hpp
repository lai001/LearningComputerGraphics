#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <Foundation/Foundation.hpp>

class FTextureDescription: public ks::noncopyable
{
public:
	~FTextureDescription();

	static FTextureDescription* New(
		const std::string& ImageFilePath,
		const aiTextureType TextureType);

	aiTextureType GetTextureType() const;

	std::string GetFilepath() const;

	const ks::PixelBuffer* GetPixelBuffer() const noexcept;

protected:
	std::string Filepath;
	ks::PixelBuffer* PixelBuffer = nullptr;
	aiTextureType TextureType;
};