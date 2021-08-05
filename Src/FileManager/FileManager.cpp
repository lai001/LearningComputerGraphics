#include "FileManager.h"
#include<algorithm>

std::string GetFolder(std::string Filename)
{
	std::string Directory;
	const size_t LastSlashIndex = Filename.rfind('\\');
	if (std::string::npos != LastSlashIndex)
	{
		Directory = Filename.substr(0, LastSlashIndex);
	}
	return Directory;
}

std::string ReplaceSepartor( std::string Filepath)
{
	std::string _Path = Filepath;
	std::replace(_Path.begin(), _Path.end(), '\\', '/');
	return _Path;
}

FFileManager::FFileManager(int arg, char ** args)
	:CurrentAppPath(ReplaceSepartor(args[0])), CurrentAppDir(ReplaceSepartor(GetFolder(args[0])))
{
	ResouceDir = CurrentAppDir.append("/Resource");
}

FFileManager::~FFileManager()
{
}

const std::string FFileManager::GetCurrentAppDir()
{
	return CurrentAppDir;
}

const std::string FFileManager::GetCurrentAppPath()
{
	return CurrentAppPath;
}

const std::string FFileManager::GetResouceDir()
{
	return ResouceDir;
}

const std::string FFileManager::GetResourcePath(const std::string Filename)
{
	std::string Path = GetResouceDir();
	Path.append("/");
	Path.append(Filename);
	return Path;
}

const std::string FFileManager::GetDir(const std::string FilePath)
{
	return FilePath.substr(0, FilePath.find_last_of('/'));
}

const std::string FFileManager::Join(const std::vector<std::string> Compoents)
{
	std::string Path = "";

	int Size = Compoents.size();
	int N = Size - 1;
	for (int i = 0; i < Size; i++)
	{
		bool bIsLast = i == N;
		Path.append(Compoents[i]);
		if (bIsLast == false)
		{
			Path.append("/");
		}
	}
	return Path;
}
