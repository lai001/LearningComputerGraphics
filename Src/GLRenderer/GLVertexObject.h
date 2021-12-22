#pragma once
#include <functional>
#include <vector>
#include <array>
#include "ThirdParty/opengl.h"
#include "ThirdParty/noncopyable.hpp"
#include "GLVertexArray.h"
#include "GLShader.h"
#include "GLIndexBuffer.h"
#include "Util.h"

class FGLVertexObject: public boost::noncopyable
{
public:

	FGLVertexObject(const void * VertexBufferData,
		const unsigned int VertexBufferDataSize,
		const unsigned int * IndexBufferData,
		const unsigned int IndicesCount,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);
	FGLVertexObject(const void * VertexBufferData,
		const unsigned int VertexBufferDataSize,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);

	template<typename VectexType>
	static FGLVertexObject* New(const std::vector<VectexType>& VertexBuffer,
		const std::vector<unsigned int>& IndexBuffer,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);

	template<typename VectexType>
	static FGLVertexObject* New(const std::vector<VectexType>& VertexBuffer,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);

	template<typename VectexType, size_t Size>
	static FGLVertexObject* New(const std::array<VectexType, Size>& VertexBuffer,
		const std::vector<unsigned int>& IndexBuffer,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);

	template<typename VectexType, size_t Size>
	static FGLVertexObject* New(const std::array<VectexType, Size>& VertexBuffer,
		const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker);

	~FGLVertexObject();

	void Bind() const;
	void Unbind() const;

private:
	FGLVertexArray* Va;
	FGLVertexBuffer* Vb;
	FGLIndexBuffer* Ib = nullptr;
};

template<typename VectexType>
inline FGLVertexObject * FGLVertexObject::New(const std::vector<VectexType>& VertexBuffer,
	const std::vector<unsigned int>& IndexBuffer,
	const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker)
{
	FGLVertexObject * VertexObject = new FGLVertexObject(VertexBuffer.data(),
		FUtil::BytesOfElements(VertexBuffer),
		IndexBuffer.data(),
		IndexBuffer.size(),
		VertexBufferLayoutMaker);
	return VertexObject;
}

template<typename VectexType>
inline FGLVertexObject * FGLVertexObject::New(const std::vector<VectexType>& VertexBuffer, 
	const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker)
{
	FGLVertexObject * VertexObject = new FGLVertexObject(VertexBuffer.data(),
		FUtil::BytesOfElements(VertexBuffer),
		VertexBufferLayoutMaker);
	return VertexObject;
}

template<typename VectexType, size_t Size>
inline FGLVertexObject * FGLVertexObject::New(const std::array<VectexType, Size>& VertexBuffer, 
	const std::vector<unsigned int>& IndexBuffer,
	const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker)
{
	FGLVertexObject * VertexObject = new FGLVertexObject(VertexBuffer.data(),
		FUtil::BytesOfElements(VertexBuffer),
		IndexBuffer.data(),
		IndexBuffer.size(),
		VertexBufferLayoutMaker);
	return VertexObject;
}

template<typename VectexType, size_t Size>
inline FGLVertexObject * FGLVertexObject::New(const std::array<VectexType, Size>& VertexBuffer,
	const std::function<const FGLVertexBufferLayout()> VertexBufferLayoutMaker)
{
	FGLVertexObject * VertexObject = new FGLVertexObject(VertexBuffer.data(),
		FUtil::BytesOfElements(VertexBuffer),
		VertexBufferLayoutMaker);
	return VertexObject;
}
