#include "SkyBox.h"

FSkyBox::FSkyBox(std::vector<std::string> FilePaths, std::string VertexShaderPath, std::string FragmentShaderPath)
	:FilePaths(FilePaths), VertexShaderPath(VertexShaderPath), FragmentShaderPath(FragmentShaderPath)
{

}

FSkyBox::~FSkyBox()
{
}
