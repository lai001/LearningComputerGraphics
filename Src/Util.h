#pragma once
#include <string>
#include <vector>

class FUtil
{
public:
	static std::string ReadFile(std::string FilePath);

	template<typename T>
	inline static size_t BytesOfElements(const std::vector<T>& Vector);

	template<typename T, size_t Size>
	inline static size_t BytesOfElements(const std::array<T, Size>& Vector);
};

template<typename T>
inline size_t FUtil::BytesOfElements(const std::vector<T>& Vector)
{
	return sizeof(T) * Vector.size();
}

template<typename T, size_t Size>
inline size_t FUtil::BytesOfElements(const std::array<T, Size>& Vector)
{
	return Size * Vector.size();
}
