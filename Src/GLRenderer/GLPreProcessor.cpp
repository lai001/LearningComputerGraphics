#include "GLPreProcessor.h"
#include <regex>
#include <iterator>

#include "ThirdParty/spdlog.h"

#include "GLShader.h"
#include "Util.h"

std::string GLPreProcessor::Process(const std::string & ShaderSourceCode)
{
	std::string CopyShaderSourceCode = ShaderSourceCode;

	std::regex IncludeRegex(R"(#\s*include\s*\S*")");
	auto Begin = std::sregex_iterator(ShaderSourceCode.begin(), ShaderSourceCode.end(), IncludeRegex);
	auto End = std::sregex_iterator();

	for (std::sregex_iterator i = Begin; i != End; ++i) {
		std::string MatchString = (*i).str();
	
		std::string RelativePath = FindPath(MatchString);
		std::string AbsPath = FGLShader::GetShadersFolder().append("/").append(RelativePath);
		std::string SourceCode = FUtil::ReadFile(AbsPath);

		std::regex ReplaceRegex(MatchString);
		std::string NewStr = std::regex_replace(CopyShaderSourceCode, ReplaceRegex, SourceCode);
		
		CopyShaderSourceCode = NewStr;
	}

	return CopyShaderSourceCode;
}

std::string GLPreProcessor::FindPath(const std::string & RawString)
{
	std::regex RelativePathRegex(R"("\S*")");
	auto Begin = std::sregex_iterator(RawString.begin(), RawString.end(), RelativePathRegex);
	auto End = std::sregex_iterator();
	for (std::sregex_iterator i = Begin; i != End; ++i) {
		std::string MatchString = (*i).str();
		return MatchString.substr(1, MatchString.size() - 2);
	}
	return "";
}
