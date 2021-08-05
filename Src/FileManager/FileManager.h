#pragma once
#include <string>
#include <vector>

class FFileManager
{
public:
	FFileManager(int arg, char** args);
	~FFileManager();

	const std::string GetCurrentAppDir();
	const std::string GetCurrentAppPath();
	const std::string GetResouceDir();
	const std::string GetResourcePath(const std::string Filename);

	const static std::string GetDir(const std::string FilePath);
	const static std::string Join(const std::vector<std::string> Compoents);

private:

	std::string CurrentAppDir;
	std::string CurrentAppPath;
	std::string ResouceDir;

};

