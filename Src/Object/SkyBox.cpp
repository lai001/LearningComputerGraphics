#include "SkyBox.hpp"
#include "ImageIO/ImageIO.hpp"

FSkyBox::FSkyBox(const std::array<std::string, 6>& FilePaths)
	:FilePaths(FilePaths)
{
	for (size_t i = 0; i < FilePaths.size(); i++)
	{
		PixelBuffers[i] = ImageIO::ReadImageFromFilePath(FilePaths[i]);
	}
}

FSkyBox::~FSkyBox()
{
}

std::array<ks::PixelBuffer*, 6> FSkyBox::GetPixelBuffers() const noexcept
{
	return PixelBuffers;
}
