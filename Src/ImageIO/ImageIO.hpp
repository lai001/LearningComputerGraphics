#ifndef ImageIO_hpp
#define ImageIO_hpp

#include <string>
#include <Foundation/Foundation.hpp>

class ImageIO
{
public:
	static ks::PixelBuffer* ReadImageFromFilePath(const std::string& path);
	static bool SaveImage(const ks::PixelBuffer& buffer, const std::string& targetPath);
};

#endif // !ImageIO_hpp