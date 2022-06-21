#pragma once
#include <array>
#include <string>
#include <glm/glm.hpp>
#include <Foundation/Foundation.hpp>
#include "TextureDescription.hpp"

class FSkyBox
{
public:
	enum SkyBoxFaceType
	{
		Left,
		Right,
		Top,
		Bottom,
		Front,
		Back
	};

private:
	std::array<ks::PixelBuffer*, 6> PixelBuffers;

public:
	FSkyBox(const std::array<std::string, 6>& FilePaths);
	~FSkyBox();

	glm::mat4 View;

	glm::mat4 Projection;

	std::array<std::string, 6> FilePaths;

	bool bIsVisible = true;

	std::string Name;

	std::array<ks::PixelBuffer*, 6> GetPixelBuffers() const noexcept;
};