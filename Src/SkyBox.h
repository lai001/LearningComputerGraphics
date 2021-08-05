#pragma once
#include <vector>
#include <string>
#include "ThirdParty/glm.h"

class FSkyBox
{

public:
	FSkyBox(std::vector<std::string> FilePaths, std::string VertexShaderPath, std::string FragmentShaderPath);
	~FSkyBox();

	glm::mat4 View;

	glm::mat4 Projection;

	std::string VertexShaderPath;

	std::string FragmentShaderPath;

	std::vector<std::string> FilePaths;
};