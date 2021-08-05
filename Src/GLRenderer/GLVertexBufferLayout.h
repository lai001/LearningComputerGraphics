#pragma once
#include <vector>
#include "ThirdParty/opengl.h"

struct FGLVertexBufferElement
{
	unsigned int Type;
	unsigned int Count;
	bool bNormalized;

	static unsigned int GetSizeOfType(unsigned int Type)
	{
		switch (Type)
		{

		case GL_FLOAT:				return 4;
		case GL_UNSIGNED_INT:		return 4;
		case GL_UNSIGNED_BYTE:		return 1;
		case GL_INT:				return 4;
		default:
			__debugbreak();
			return 0;
		}
	}
};

class FGLVertexBufferLayout
{

private:
	std::vector<FGLVertexBufferElement> Elements;
	unsigned int Stride = 0;

public:
	FGLVertexBufferLayout();

	template<typename T>
	void Push(unsigned int Count)
	{
		static_assert(false);
	}

	template<>
	void Push<int>(unsigned int Count)
	{
		Elements.push_back({ GL_INT, Count, false });
		Stride += Count * sizeof(int);
	}

	template<>
	void Push<float>(unsigned int Count)
	{
		Elements.push_back({ GL_FLOAT, Count, false });
		Stride += Count * sizeof(float);
	}

	template<>
	void Push<unsigned int>(unsigned int Count)
	{
		Elements.push_back({ GL_UNSIGNED_INT, Count, false });
		Stride += Count * sizeof(unsigned int);
	}

	template<>
	void Push<unsigned char>(unsigned int Count)
	{
		Elements.push_back({ GL_UNSIGNED_BYTE, Count, false });
		Stride += Count * sizeof(unsigned char);
	}

	inline std::vector<FGLVertexBufferElement> GetElements() const
	{
		return Elements;
	}

	inline unsigned int GetStride() const
	{
		return Stride;
	}
};