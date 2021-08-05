#pragma once
#include "GLVertexBuffer.h"
#include "GLVertexBufferLayout.h"

class FGLVertexArray
{
private:
	unsigned int RendererID;
public:
	FGLVertexArray();
	~FGLVertexArray();

	int GetRendererID();

	void AddBuffer(const FGLVertexBuffer* Vb, const FGLVertexBufferLayout Layout);

	void Bind() const;
	void Unbind();
};
