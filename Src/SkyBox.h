#pragma once
#include <vector>
#include <string>
#include "ThirdParty/glm.h"

class FSkyBox
{
public:
	FSkyBox(std::vector<std::string> FilePaths);
	~FSkyBox();

	glm::mat4 View;

	glm::mat4 Projection;

	std::vector<std::string> FilePaths;

	bool bIsVisible = true;

	std::string Name;
};