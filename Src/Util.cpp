#include "Util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "ThirdParty/spdlog.h"

std::string FUtil::ReadFile(std::string FilePath)
{
	std::string SourceShader;
	std::ifstream InputFileStream;
	std::stringstream ShaderStream;

	InputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		InputFileStream.open(FilePath);
		ShaderStream << InputFileStream.rdbuf();
		SourceShader = ShaderStream.str();
		InputFileStream.close();
	}
	catch (std::ifstream::failure E)
	{
		spdlog::error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ IN FILE: {0}", FilePath);
	}
	return SourceShader;
}
