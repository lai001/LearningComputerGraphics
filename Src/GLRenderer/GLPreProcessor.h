#pragma once
#include <string>
#include <vector>
class GLPreProcessor
{
public:
	static std::string Process(const std::string& ShaderSourceCode);
	static std::string FindPath(const std::string& RawString);
};

